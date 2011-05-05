#include "nodetree.h"

#include <stdio.h>
#include <string>

void WalkNode(const aiNode *node);
void ShowMeshInfo(const aiMesh *mesh, const aiScene *scene);
void ShowMaterialInfo(const aiMaterial *material);
void ShowMaterialTextureInfo(const aiMaterial *material, aiTextureType type, const std::string &typeText);
void ShowAnimationInfo(const aiAnimation *animation);

unsigned int g_level;
FILE *g_fp = NULL;

std::string GetIndentString()
{
	std::string indent;
	for (unsigned int i = 0; i < g_level; ++i)
		indent += "    ";
	return indent;
}

void Walk(const aiScene *scene, const std::string &filename)
{
	g_level = 0;

	g_fp = fopen(filename.c_str(), "w");
	if (g_fp == NULL)
		throw std::exception("Error creating description output file.");

	fprintf(g_fp, "\n*** GENERAL SCENE INFO ***\n\n");
	fprintf(g_fp, "HasAnimations:  %s\n", scene->HasAnimations() ? "yes" : "no");
	fprintf(g_fp, "HasMaterials:   %s\n", scene->HasMaterials() ? "yes" : "no");
	fprintf(g_fp, "HasMeshes:      %s\n", scene->HasMeshes() ? "yes" : "no");
	fprintf(g_fp, "mNumAnimations: %d\n", scene->mNumAnimations);
	fprintf(g_fp, "mNumMaterials:  %d\n", scene->mNumMaterials);
	fprintf(g_fp, "mNumMeshes:     %d\n", scene->mNumMeshes);
	fprintf(g_fp, "\n");

	if (scene->mNumMaterials > 0)
	{
		fprintf(g_fp, "\n*** MATERIALS ***\n\n");

		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			fprintf(g_fp, "[%d]:\n", i);
			ShowMaterialInfo(scene->mMaterials[i]);
		}
	}

	fprintf(g_fp, "\n*** MESHES ***\n\n");

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		fprintf(g_fp, "[%d]\n", i);
		ShowMeshInfo(scene->mMeshes[i], scene);
	}

	fprintf(g_fp, "\n*** NODES ***\n");
	WalkNode(scene->mRootNode);

	if (scene->mNumAnimations > 0)
	{
		fprintf(g_fp, "\n*** ANIMATIONS ***\n\n");

		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			fprintf(g_fp, "[%d]:\n", i);
			ShowAnimationInfo(scene->mAnimations[i]);
		}
	}

	fclose(g_fp);
	g_fp = NULL;
}

void WalkNode(const aiNode *node)
{
	++g_level;
	fprintf(g_fp, "\n");

	std::string indent = GetIndentString();

	aiQuaternion rotation;
	aiVector3D scaling;
	aiVector3D position;
	node->mTransformation.Decompose(scaling, rotation, position);

	fprintf(g_fp, "%sName:         %s\n", indent.c_str(), node->mName.data);
	fprintf(g_fp, "%smNumChildren: %d\n", indent.c_str(), node->mNumChildren);
	fprintf(g_fp, "%smNumMeshes:   %d", indent.c_str(), node->mNumMeshes);
	if (node->mNumMeshes > 0)
	{
		fprintf(g_fp, " [");
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			if (i > 0)
				fprintf(g_fp, ", ");
			fprintf(g_fp, "%d", node->mMeshes[i]);
		}
		fprintf(g_fp, "]");
	}
	fprintf(g_fp, "\n");
	fprintf(g_fp, "%smTransformation:\n", indent.c_str());
	fprintf(g_fp, "%s - Rotation:     %f, %f, %f, %f\n", indent.c_str(), rotation.x, rotation.y, rotation.z, rotation.w);
	fprintf(g_fp, "%s - Scaling:      %f, %f, %f\n", indent.c_str(), scaling.x, scaling.y, scaling.z);
	fprintf(g_fp, "%s - Position:     %f, %f, %f\n", indent.c_str(), position.x, position.y, position.z);

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		WalkNode(node->mChildren[i]);

	--g_level;
}

void ShowMeshInfo(const aiMesh *mesh, const aiScene *scene)
{
	fprintf(g_fp, "    Name:                     %s\n", mesh->mName.data);
	fprintf(g_fp, "    HasBones:                 %s\n", mesh->HasBones() ? "yes" : "no");
	fprintf(g_fp, "    HasFaces:                 %s\n", mesh->HasFaces() ? "yes" : "no");
	fprintf(g_fp, "    HasNormals:               %s\n", mesh->HasNormals() ? "yes" : "no");
	fprintf(g_fp, "    HasPositions:             %s\n", mesh->HasPositions() ? "yes" : "no");
	fprintf(g_fp, "    HasTangentsAndBitangents: %s\n", mesh->HasTangentsAndBitangents() ? "yes" : "no");
	fprintf(g_fp, "    HasTextureCoords:         %s, %s, %s, %s\n",
		(mesh->HasTextureCoords(0) ? "yes" : "no"),
		(mesh->HasTextureCoords(1) ? "yes" : "no"),
		(mesh->HasTextureCoords(2) ? "yes" : "no"),
		(mesh->HasTextureCoords(3) ? "yes" : "no")
		);
	fprintf(g_fp, "    HasVertexColors:          %s, %s, %s, %s\n",
		(mesh->HasVertexColors(0) ? "yes" : "no"),
		(mesh->HasVertexColors(1) ? "yes" : "no"),
		(mesh->HasVertexColors(2) ? "yes" : "no"),
		(mesh->HasVertexColors(3) ? "yes" : "no")
		);
	fprintf(g_fp, "    mMaterialIndex:           %d\n", mesh->mMaterialIndex);
	fprintf(g_fp, "    mNumFaces:                %d\n", mesh->mNumFaces);
	fprintf(g_fp, "    mPrimitiveTypes:          %d - (All Triangles? %s)\n", mesh->mPrimitiveTypes, mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE ? "yes" : "no");
	fprintf(g_fp, "    mNumVertices:             %d\n", mesh->mNumVertices);
	fprintf(g_fp, "    mNumUVComponents:         %d, %d, %d, %d\n", mesh->mNumUVComponents[0], mesh->mNumUVComponents[1], mesh->mNumUVComponents[2], mesh->mNumUVComponents[3]);
	fprintf(g_fp, "    mNumBones:                %d\n", mesh->mNumBones);
	fprintf(g_fp, "    mNumAnimMeshes:           %d\n", mesh->mNumAnimMeshes);
	fprintf(g_fp, "    mBones:\n");
	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		aiBone *bone = mesh->mBones[i];

		aiQuaternion rotation;
		aiVector3D scaling;
		aiVector3D position;
		bone->mOffsetMatrix.Decompose(scaling, rotation, position);

		aiString parentName = "[null]";
		aiNode *boneNode = scene->mRootNode->FindNode(bone->mName);
		if (boneNode != NULL)
		{
			aiNode *parent = boneNode->mParent;
			if (parent != NULL)
				parentName = parent->mName;
		}

		fprintf(g_fp, "        [%d]\n", i);
		fprintf(g_fp, "            Name:        %s\n", bone->mName.data);
		fprintf(g_fp, "            Parent Name: %s\n", parentName.data);
		fprintf(g_fp, "            mNumWeights: %d\n", bone->mNumWeights);
		fprintf(g_fp, "            mOffsetMatrix:\n");
		fprintf(g_fp, "             - Rotation:    %f, %f, %f, %f\n", rotation.x, rotation.y, rotation.z, rotation.w);
		fprintf(g_fp, "             - Scaling:     %f, %f, %f\n", scaling.x, scaling.y, scaling.z);
		fprintf(g_fp, "             - Position:    %f, %f, %f\n", position.x, position.y, position.z);
	}
}

void ShowMaterialInfo(const aiMaterial *material)
{
	aiString name;
	aiString textureFile;
	aiColor3D ambient;
	aiColor3D diffuse;
	aiColor3D specular;
	aiColor3D emissive;
	float shininess;
	float opacity;

	material->Get(AI_MATKEY_NAME, name);
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
	material->Get(AI_MATKEY_SHININESS, shininess);
	material->Get(AI_MATKEY_OPACITY, opacity);

	fprintf(g_fp, "    Name:      %s\n", name.data);
	fprintf(g_fp, "    Ambient:   %f, %f, %f\n", ambient.r, ambient.g, ambient.b);
	fprintf(g_fp, "    Diffuse:   %f, %f, %f\n", diffuse.r, diffuse.g, diffuse.b);
	fprintf(g_fp, "    Specular:  %f, %f, %f\n", specular.r, specular.g, specular.b);
	fprintf(g_fp, "    Emissive:  %f, %f, %f\n", emissive.r, emissive.g, emissive.b);
	fprintf(g_fp, "    Shininess: %f\n", shininess);
	fprintf(g_fp, "    Opacity:   %f\n", opacity);

	ShowMaterialTextureInfo(material, aiTextureType_NONE, "aiTextureType_NONE");
	ShowMaterialTextureInfo(material, aiTextureType_DIFFUSE, "aiTextureType_DIFFUSE");
	ShowMaterialTextureInfo(material, aiTextureType_SPECULAR, "aiTextureType_SPECULAR");
	ShowMaterialTextureInfo(material, aiTextureType_AMBIENT, "aiTextureType_AMBIENT");
	ShowMaterialTextureInfo(material, aiTextureType_EMISSIVE, "aiTextureType_EMISSIVE");
	ShowMaterialTextureInfo(material, aiTextureType_HEIGHT, "aiTextureType_HEIGHT");
	ShowMaterialTextureInfo(material, aiTextureType_NORMALS, "aiTextureType_NORMALS");
	ShowMaterialTextureInfo(material, aiTextureType_SHININESS, "aiTextureType_SHININESS");
	ShowMaterialTextureInfo(material, aiTextureType_OPACITY, "aiTextureType_OPACITY");
	ShowMaterialTextureInfo(material, aiTextureType_DISPLACEMENT, "aiTextureType_DISPLACEMENT");
	ShowMaterialTextureInfo(material, aiTextureType_LIGHTMAP, "aiTextureType_LIGHTMAP");
	ShowMaterialTextureInfo(material, aiTextureType_REFLECTION, "aiTextureType_REFLECTION");
	ShowMaterialTextureInfo(material, aiTextureType_UNKNOWN, "aiTextureType_UNKNOWN");
}

void ShowMaterialTextureInfo(const aiMaterial *material, aiTextureType type, const std::string &typeText)
{
	aiString textureFile;

	unsigned int count = material->GetTextureCount(type);
	if (count > 0)
	{
		fprintf(g_fp, "    %s: %d\n", typeText.c_str(), count);
		for (unsigned int i = 0; i < count; ++i)
		{
			material->Get(AI_MATKEY_TEXTURE(type, i), textureFile);
			fprintf(g_fp, "        %s\n", textureFile.data);
		}
	}
}

void ShowAnimationInfo(const aiAnimation *animation)
{
	fprintf(g_fp, "    Name:             %s\n", animation->mName.data);
	fprintf(g_fp, "    mDuration:        %f\n", animation->mDuration);
	fprintf(g_fp, "    mTicksPerSecond:  %f\n", animation->mTicksPerSecond);
	fprintf(g_fp, "    mNumChannels:     %d\n", animation->mNumChannels);
	fprintf(g_fp, "    mChannels:\n");
	for (unsigned int i = 0; i < animation->mNumChannels; ++i)
	{
		aiNodeAnim *channel = animation->mChannels[i];

		fprintf(g_fp, "        [%d]:\n", i);
		fprintf(g_fp, "            Name:             %s\n", channel->mNodeName.data);
		fprintf(g_fp, "            mNumPositionKeys: %d\n", channel->mNumPositionKeys);
		fprintf(g_fp, "            mNumRotationKeys: %d\n", channel->mNumRotationKeys);
		fprintf(g_fp, "            mNumScalingKeys:  %d\n", channel->mNumScalingKeys);
	}
	fprintf(g_fp, "    mNumMeshChannels: %d\n", animation->mNumMeshChannels);
	fprintf(g_fp, "    mMeshChannels:\n");
	for (unsigned int i = 0; i < animation->mNumMeshChannels; ++i)
	{
		aiMeshAnim *channel = animation->mMeshChannels[i];

		fprintf(g_fp, "        [%d]:\n", i);
		fprintf(g_fp, "            Name:     %s\n", channel->mName.data);
		fprintf(g_fp, "            mNumKeys: %d\n", channel->mNumKeys);
	}
}
