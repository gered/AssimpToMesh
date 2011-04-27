#ifndef __ASSIMPUTILS_UTILS_H_INCLUDED__
#define __ASSIMPUTILS_UTILS_H_INCLUDED__

#include <assimp.hpp>
#include <aiScene.h>

#include <map>
#include <vector>

#include "types.h"

bool IsSceneAnimated(const aiScene *scene);
bool IsSceneSkeletal(const aiScene *scene);
bool IsSceneStatic(const aiScene *scene);

/**
 * This will go through each mesh in the scene, and store all the vertices it 
 * finds into one common bucket, building up a mapping as it goes so that,
 * we can easily determine what mesh index and vertex index as stored by ASSIMP
 * corresponds to what vertex in the common bucket. That is, after this, given:
 *
 *    aiScene *scene = ...
 *    aiMesh *mesh = scene->mMeshes[meshIndex];
 *    unsigned int bucketIndex = mesh->mVertices[3];
 *    ... and/or ...
 *    unsigned int bucketIndex = mesh->mFaces[10]->mIndices[0];
 *    aiVector3D vertex = bucket[bucketIndex];
 *
 * where "bucketIndex" is the index into one of the bucket lists
 */
void CollectAllMeshVertices(
	const aiScene *scene, 
	AssimpVertices &verticesBucket, 
	AssimpVertices &normalsBucket, 
	AssimpVertices &texCoordsBucket,
	AssimpVerticesMap &indexMapping);

#endif
