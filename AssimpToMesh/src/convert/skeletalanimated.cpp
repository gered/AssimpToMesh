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

	// joints
	// foreach joint:
	//  - name
	//  - parentindex (or name?)
	//  - position (vec3d)
	//  - rotation (vec3d or quat?)

	// joints to vertices mapping
	// foreach vertex:
	//  - jointindex
	//  - weight

	// joint keyframes
	// foreach keyframe:
	//  foreach joint:
	//   - position (vec3d)
	//   - rotation (vec3d or quat?)

	// animation sequences
	// foreach sequence:
	//  - name
	//  - start frame index
	//  - end frame index

	fclose(fp);
}
