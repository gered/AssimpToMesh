#ifndef __CONVERT_MESHTRIANGLE_H_INCLUDED__
#define __CONVERT_MESHTRIANGLE_H_INCLUDED__

#include <stdint.h>

struct MeshTriangle
{
	uint32_t vertices[3];
	uint32_t subMesh;
};

#endif
