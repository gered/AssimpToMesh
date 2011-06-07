#include "convert.h"

#include <stdio.h>
#include <stdint.h>
#include <exception>

#include "mesh.h"
#include "../assimputils/utils.h"

void ConvertSkeletalAnimated(const std::string &outfile, const aiScene *scene)
{
	FILE *fp = fopen(outfile.c_str(), "wb");
	if (fp == NULL)
		throw std::exception("Error creating output file.");

	WriteMeshHeader(fp);

	// basic mesh info
	AssimpVertices vertices;
	AssimpVertices normals;
	AssimpVertices texCoords;
	AssimpVerticesMap vertexIndicesMap;
	CollectAllMeshVertices(scene, vertices, normals, texCoords, vertexIndicesMap);
	WriteVertices(vertices, fp);
	WriteNormals(normals, fp);
	WriteTexCoords(texCoords, fp);

	// materials
	std::vector<MeshMaterial> materials;
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		MeshMaterial material = MeshMaterial(scene->mMaterials[i]);
		materials.push_back(material);
	}
	WriteMaterials(materials, fp);

	// triangles
	std::vector<MeshTriangle> triangles;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
			aiFace *face = &mesh->mFaces[j];

			MeshTriangle triangle;
			triangle.vertices[0] = vertexIndicesMap[i][face->mIndices[0]];
			triangle.vertices[1] = vertexIndicesMap[i][face->mIndices[1]];
			triangle.vertices[2] = vertexIndicesMap[i][face->mIndices[2]];
			triangle.subMesh = i;

			triangles.push_back(triangle);
		}
	}
	WriteTriangles(triangles, fp);

	// sub meshes
	std::vector<SubMesh> subMeshes;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		SubMesh subMesh;

		subMesh.name = std::string(mesh->mName.data, mesh->mName.length);
		subMesh.material = mesh->mMaterialIndex;
		subMesh.numTriangles = mesh->mNumFaces;

		subMeshes.push_back(subMesh);
	}
	WriteSubMeshes(subMeshes, fp);

	// TODO: cheating for now, but according to the ASSIMP doc's, there could be
	//       models which have nodes that are not used by any bones, but still
	//       affect the skeleton pose, because child nodes *are* used by bones
	//       and so, any parent node transformations will apply and need to be
	//       carried down the chain. None of the test models I'm using have this
	//       type of node structure, so I'm ignoring it for now :)

	// collect basic skeleton/joint info
	std::vector<MeshJoint> joints;
	std::vector<uint32_t> vertexToJointMap;
	vertexToJointMap.resize(vertices.size());
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		for (unsigned int j = 0; j < mesh->mNumBones; ++j)
		{
			aiBone *bone = mesh->mBones[j];
			aiNode *node = scene->mRootNode->FindNode(bone->mName);
			aiNode *parentNode = node->mParent;

			MeshJoint joint;
			joint.name = std::string(bone->mName.data, bone->mName.length);
			if (strcmp(parentNode->mName.data, "$dummy_root") == 0)
				joint.parentName = "";
			else
				joint.parentName = std::string(parentNode->mName.data, parentNode->mName.length);

			aiMatrix4x4 transform = node->mTransformation * bone->mOffsetMatrix;
			node->mTransformation.DecomposeNoScaling(joint.localRotation, joint.localPosition);
			bone->mOffsetMatrix.DecomposeNoScaling(joint.offsetRotation, joint.offsetPosition);

			// assumption: sum of all bone->mNumWeights is always == vertices.size()
			for (unsigned int k = 0; k < bone->mNumWeights; ++k)
			{
				aiVertexWeight *weight = &bone->mWeights[k];
				unsigned int index = vertexIndicesMap[i][weight->mVertexId];
				vertexToJointMap[index] = j;
			}

			joints.push_back(joint);
		}
	}
	WriteJoints(joints, fp);
	WriteJointToVertexMap(vertexToJointMap, fp);

	// TODO: this collection process is dumb at the moment. assumes each bone
	//       has the same number of frames as all the other ones. also, every
	//       test model i've used so far puts all the frames into a single
	//       animation (usually without a name even). so this is, for now, only
	//       looking at the first aiAnimation object. ALSO, because i'm REALLY
	//       lazy, the animations aiNodeAnim objects are assumed to be in the
	//       same order as the bones were collected in :)
	// collect joint keyframes
	std::vector<JointKeyFrames> meshFrames;
	aiAnimation *animation = scene->mAnimations[0];
	for (unsigned int i = 0; i < animation->mNumChannels; ++i)
	{
		aiNodeAnim *nodeAnim = animation->mChannels[i];
		JointKeyFrames jointFrames;

		for (unsigned int j = 0; j < nodeAnim->mNumPositionKeys; ++j)
		{
			// TODO: should really get the times too
			MeshJointKeyFrame frame;
			frame.position = nodeAnim->mPositionKeys[j].mValue;
			frame.rotation = nodeAnim->mRotationKeys[j].mValue;

			jointFrames.push_back(frame);
		}

		meshFrames.push_back(jointFrames);
	}
	WriteJointKeyFrames(meshFrames, fp);

	fclose(fp);
}
