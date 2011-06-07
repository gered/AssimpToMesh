#ifndef __CONVERT_SUBMESH_H_INCLUDED__
#define __CONVERT_SUBMESH_H_INCLUDED__

#include <stdint.h>
#include <string>

struct SubMesh
{
	std::string name;
	uint32_t material;
	uint32_t numTriangles;
};

#endif
