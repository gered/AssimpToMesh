#include "nodetree.h"

#include <stdio.h>
#include <string>

void WalkNode(const aiNode *node);
void ShowMeshInfo(const aiMesh *mesh, const aiScene *scene);
void ShowMaterialInfo(const aiMaterial *material);
void ShowMaterialTextureInfo(const aiMaterial *material, aiTextureType type, const std::string &typeText);
void ShowAnimationInfo(const aiAnimation *animation);

unsigned int g_level;

std::string GetIndentString()
{
	std::string indent;
	for (unsigned int i = 0; i < g_level; ++i)
		indent += "    ";
	return indent;
}

void Walk(const aiScene *scene)
{
	g_level = 0;

	printf("\n*** GENERAL SCENE INFO ***\n\n");
	printf("HasAnimations:  %s\n", scene->HasAnimations() ? "yes" : "no");
	printf("HasMaterials:   %s\n", scene->HasMaterials() ? "yes" : "no");
	printf("HasMeshes:      %s\n", scene->HasMeshes() ? "yes" : "no");
	printf("mNumAnimations: %d\n", scene->mNumAnimations);
	printf("mNumMaterials:  %d\n", scene->mNumMaterials);
	printf("mNumMeshes:     %d\n", scene->mNumMeshes);
	printf("\n");

	if (scene->mNumMaterials > 0)
	{
		printf("\n*** MATERIALS ***\n\n");

		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			printf("[%d]:\n", i);
			ShowMaterialInfo(scene->mMaterials[i]);
		}
	}

	printf("\n*** MESHES ***\n\n");

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		printf("[%d]\n", i);
		ShowMeshInfo(scene->mMeshes[i], scene);
	}

	printf("\n*** NODES ***\n");
	WalkNode(scene->mRootNode);

	if (scene->mNumAnimations > 0)
	{
		printf("\n*** ANIMATIONS ***\n\n");

		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			printf("[%d]:\n", i);
			ShowAnimationInfo(scene->mAnimations[i]);
		}
	}

	printf("\n");
}

void WalkNode(const aiNode *node)
{
	++g_level;
	printf("\n");

	std::string indent = GetIndentString();

	aiQuaternion rotation;
	aiVector3D scaling;
	aiVector3D position;
	node->mTransformation.Decompose(scaling, rotation, position);

	printf("%sName:         %s\n", indent.c_str(), node->mName.data);
	printf("%smNumChildren: %d\n", indent.c_str(), node->mNumChildren);
	printf("%smNumMeshes:   %d", indent.c_str(), node->mNumMeshes);
	if (node->mNumMeshes > 0)
	{
		printf(" [");
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			if (i > 0)
				printf(", ");
			printf("%d", node->mMeshes[i]);
		}
		printf("]");
	}
	printf("\n");
	printf("%smTransformation:\n", indent.c_str());
	printf("%s - Rotation:     %f, %f, %f, %f\n", indent.c_str(), rotation.x, rotation.y, rotation.z, rotation.w);
	printf("%s - Scaling:      %f, %f, %f\n", indent.c_str(), scaling.x, scaling.y, scaling.z);
	printf("%s - Position:     %f, %f, %f\n", indent.c_str(), position.x, position.y, position.z);

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		WalkNode(node->mChildren[i]);

	--g_level;
}

void ShowMeshInfo(const aiMesh *mesh, const aiScene *scene)
{
	printf("    Name:                     %s\n", mesh->mName.data);
	printf("    HasBones:                 %s\n", mesh->HasBones() ? "yes" : "no");
	printf("    HasFaces:                 %s\n", mesh->HasFaces() ? "yes" : "no");
	printf("    HasNormals:               %s\n", mesh->HasNormals() ? "yes" : "no");
	printf("    HasPositions:             %s\n", mesh->HasPositions() ? "yes" : "no");
	printf("    HasTangentsAndBitangents: %s\n", mesh->HasTangentsAndBitangents() ? "yes" : "no");
	printf("    HasTextureCoords:         %s, %s, %s, %s\n",
		(mesh->HasTextureCoords(0) ? "yes" : "no"),
		(mesh->HasTextureCoords(1) ? "yes" : "no"),
		(mesh->HasTextureCoords(2) ? "yes" : "no"),
		(mesh->HasTextureCoords(3) ? "yes" : "no")
		);
	printf("    HasVertexColors:          %s, %s, %s, %s\n",
		(mesh->HasVertexColors(0) ? "yes" : "no"),
		(mesh->HasVertexColors(1) ? "yes" : "no"),
		(mesh->HasVertexColors(2) ? "yes" : "no"),
		(mesh->HasVertexColors(3) ? "yes" : "no")
		);
	printf("    mMaterialIndex:           %d\n", mesh->mMaterialIndex);
	printf("    mNumFaces:                %d\n", mesh->mNumFaces);
	printf("    mPrimitiveTypes:          %d - (All Triangles? %s)\n", mesh->mPrimitiveTypes, mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE ? "yes" : "no");
	printf("    mNumVertices:             %d\n", mesh->mNumVertices);
	printf("    mNumUVComponents:         %d, %d, %d, %d\n", mesh->mNumUVComponents[0], mesh->mNumUVComponents[1], mesh->mNumUVComponents[2], mesh->mNumUVComponents[3]);
	printf("    mNumBones:                %d\n", mesh->mNumBones);
	printf("    mNumAnimMeshes:           %d\n", mesh->mNumAnimMeshes);
	printf("    mBones:\n");
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

		printf("        [%d]\n", i);
		printf("            Name:        %s\n", bone->mName.data);
		printf("            Parent Name: %s\n", parentName.data);
		printf("            mNumWeights: %d\n", bone->mNumWeights);
		printf("            mOffsetMatrix:\n");
		printf("             - Rotation:    %f, %f, %f, %f\n", rotation.x, rotation.y, rotation.z, rotation.w);
		printf("             - Scaling:     %f, %f, %f\n", scaling.x, scaling.y, scaling.z);
		printf("             - Position:    %f, %f, %f\n", position.x, position.y, position.z);
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

	printf("    Name:      %s\n", name.data);
	printf("    Ambient:   %f, %f, %f\n", ambient.r, ambient.g, ambient.b);
	printf("    Diffuse:   %f, %f, %f\n", diffuse.r, diffuse.g, diffuse.b);
	printf("    Specular:  %f, %f, %f\n", specular.r, specular.g, specular.b);
	printf("    Emissive:  %f, %f, %f\n", emissive.r, emissive.g, emissive.b);
	printf("    Shininess: %f\n", shininess);
	printf("    Opacity:   %f\n", opacity);

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
		printf("    %s: %d\n", typeText.c_str(), count);
		for (unsigned int i = 0; i < count; ++i)
		{
			material->Get(AI_MATKEY_TEXTURE(type, i), textureFile);
			printf("        %s\n", textureFile.data);
		}
	}
}

void ShowAnimationInfo(const aiAnimation *animation)
{
	printf("    Name:             %s\n", animation->mName.data);
	printf("    mDuration:        %f\n", animation->mDuration);
	printf("    mTicksPerSecond:  %f\n", animation->mTicksPerSecond);
	printf("    mNumChannels:     %d\n", animation->mNumChannels);
	printf("    mChannels:\n");
	for (unsigned int i = 0; i < animation->mNumChannels; ++i)
	{
		aiNodeAnim *channel = animation->mChannels[i];

		printf("        [%d]:\n", i);
		printf("            Name:             %s\n", channel->mNodeName.data);
		printf("            mNumPositionKeys: %d\n", channel->mNumPositionKeys);
		printf("            mNumRotationKeys: %d\n", channel->mNumRotationKeys);
		printf("            mNumScalingKeys:  %d\n", channel->mNumScalingKeys);
	}
	printf("    mNumMeshChannels: %d\n", animation->mNumMeshChannels);
	printf("    mMeshChannels:\n");
	for (unsigned int i = 0; i < animation->mNumMeshChannels; ++i)
	{
		aiMeshAnim *channel = animation->mMeshChannels[i];

		printf("        [%d]:\n", i);
		printf("            Name:     %s\n", channel->mName.data);
		printf("            mNumKeys: %d\n", channel->mNumKeys);
	}
}
