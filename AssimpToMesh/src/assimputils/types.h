#ifndef __ASSIMPUTILS_TYPES_H_INCLUDED__
#define __ASSIMPUTILS_TYPES_H_INCLUDED__

#include <aiTypes.h>
#include <vector>
#include <map>

typedef std::vector<aiVector3D> AssimpVertices;

typedef std::vector<unsigned int> AssimpVertexIndices;
typedef std::map<unsigned int, AssimpVertexIndices> AssimpVerticesMap;

#endif
