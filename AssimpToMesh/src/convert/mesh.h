#ifndef __CONVERT_MESH_H_INCLUDED__
#define __CONVERT_MESH_H_INCLUDED__

#include <stdio.h>
#include <vector>
#include <aiTypes.h>
#include "../assimputils/types.h"
#include "meshmaterial.h"
#include "meshtriangle.h"
#include "submesh.h"
#include "meshjoint.h"

void WriteMeshHeader(FILE *fp);
void WriteVertices(const AssimpVertices &vertices, FILE *fp);
void WriteNormals(const AssimpVertices &normals, FILE *fp);
void WriteTexCoords(const AssimpVertices &texCoords, FILE *fp);
void WriteMaterials(const std::vector<MeshMaterial> &materials, FILE *fp);
void WriteTriangles(const std::vector<MeshTriangle> &triangles, FILE *fp);
void WriteSubMeshes(const std::vector<SubMesh> &subMeshes, FILE *fp);
void WriteJoints(const std::vector<MeshJoint> &joints, FILE *fp);
void WriteJointToVertexMap(const std::vector<uint32_t> &vertexToJointMap, FILE *fp);
void WriteJointKeyFrames(const std::vector<JointKeyFrames> &jointKeyFrames, FILE *fp);

#endif
