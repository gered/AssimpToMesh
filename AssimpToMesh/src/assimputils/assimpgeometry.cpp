#include "utils.h"

// determine if a vertex is already contained in the bucket, if so matchingIndex will contain the index of the matching vertex
bool FindVertex(const aiVector3D &vertex, const AssimpVertices &bucket, unsigned int &matchingIndex)
{
	for (unsigned int i = 0; i < bucket.size(); ++i)
	{
		if (vertex == bucket[i])
		{
			matchingIndex = i;
			return true;
		}
	}

	return false;
}

void CollectVerticesInMesh(
	const aiMesh *mesh, 
	AssimpVertices &verticesBucket, 
	AssimpVertices &normalsBucket, 
	AssimpVertices &texCoordsBucket,
	AssimpVertexIndices &indexMapping)
{
	bool hasTexCoords = mesh->HasTextureCoords(0);
	bool hasNormals = mesh->HasNormals();

	indexMapping.resize(mesh->mNumVertices);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		unsigned int matchingIndex = 0;
		aiVector3D vertex = mesh->mVertices[i];

		// TODO: this seems to not work quite so well... might have something to do with the
		//       fact that ASSIMP is already taking care of duplicate vertices and indexing as
		//       appropriate? the problem seems to be only with tex coords though....
		//if (!FindVertex(vertex, verticesBucket, matchingIndex))
		if (true)
		{
			// vertex isn't in the bucket already, add it, and then record mapping between the mesh vertex index and the bucket vertex index
			verticesBucket.push_back(vertex);
			matchingIndex = verticesBucket.size() - 1;

			// also copy the normal and tex coord into the appropriate buckets
			// TODO: check if mesh has normals and/or tex coords first!
			if (hasNormals)
			{
				aiVector3D normal = mesh->mNormals[i];
				normalsBucket.push_back(normal);
			}
			if (hasTexCoords)
			{
				aiVector3D texCoord = mesh->mTextureCoords[0][i];
				texCoordsBucket.push_back(texCoord);
			}
		}

		// stating the obvious so when i come back here and read this, i know instantly
		// what the fuck is going on (sad, I know):
		// i = index into mesh's separate vertices list
		// matchingIndex = index into bucket's vertices list
		indexMapping[i] = matchingIndex;
	}
}

void CollectAllMeshVertices(
	const aiScene *scene, 
	AssimpVertices &verticesBucket, 
	AssimpVertices &normalsBucket, 
	AssimpVertices &texCoordsBucket,
	AssimpVerticesMap &indexMapping)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[i];
		AssimpVertexIndices indexes;
		CollectVerticesInMesh(mesh, verticesBucket, normalsBucket, texCoordsBucket, indexes);

		indexMapping[i] = indexes;
	}
}
