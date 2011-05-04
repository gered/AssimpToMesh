#include "mesh.h"

#include <stdio.h>
#include <stdint.h>

void WriteMeshHeader(FILE *fp)
{
	fputs("MESH", fp);
	uint8_t version = 1;
	fwrite(&version, 1, 1, fp);
}

void WriteVertices(const AssimpVertices &vertices, FILE *fp)
{
	uint32_t count = vertices.size();
	if (count == 0)
		return;

	uint32_t size = 
		(sizeof(float) * 3) *   // x y, z
		vertices.size()
		+ 4;                    // chunk size

	fputs("VTX", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);

	for (uint32_t i = 0; i < count; ++i)
	{
		fwrite(&vertices[i].x, sizeof(float), 1, fp);
		fwrite(&vertices[i].y, sizeof(float), 1, fp);
		fwrite(&vertices[i].z, sizeof(float), 1, fp);
	}
}

void WriteNormals(const AssimpVertices &normals, FILE *fp)
{
	uint32_t count = normals.size();
	if (count == 0)
		return;

	uint32_t size = 
		(sizeof(float) * 3) *   // x y, z
		normals.size()
		+ 4;                    // chunk size

	fputs("NRL", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);

	for (uint32_t i = 0; i < count; ++i)
	{
		fwrite(&normals[i].x, sizeof(float), 1, fp);
		fwrite(&normals[i].y, sizeof(float), 1, fp);
		fwrite(&normals[i].z, sizeof(float), 1, fp);
	}
}

void WriteTexCoords(const AssimpVertices &texCoords, FILE *fp)
{
	uint32_t count = texCoords.size();
	if (count == 0)
		return;

	uint32_t size = 
		(sizeof(float) * 2) *   // u, v
		texCoords.size()
		+ 4;                    // count

	fputs("TXT", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);

	for (uint32_t i = 0; i < count; ++i)
	{
		fwrite(&texCoords[i].x, sizeof(float), 1, fp);
		fwrite(&texCoords[i].y, sizeof(float), 1, fp);
	}
}

void WriteMaterials(const std::vector<MeshMaterial> &materials, FILE *fp)
{
	uint32_t count = materials.size();
	if (count == 0)
		return;

	uint32_t size = 4 +   // count
		(
			(
				(sizeof(float) * 3) +  // ambient
				(sizeof(float) * 3) +  // diffuse
				(sizeof(float) * 3) +  // specular
				(sizeof(float) * 3) +  // emissive
				sizeof(float) +        // shininess
				sizeof(float)          // opacity
			) * count
		);

	// add up all the variable length string sizes for the material names and texture filenames
	for (uint32_t i = 0; i < count; ++i)
	{
		size += materials[i].name.length() + 1;    // include null terminator
		size += materials[i].texture.length() + 1; // ditto
	}

	fputs("MTL", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);

	for (uint32_t i = 0; i < count; ++i)
	{
		const MeshMaterial *m = &materials[i];

		fwrite(m->name.c_str(), m->name.length(), 1, fp);
		char ch = '\0';
		fwrite(&ch, 1, 1, fp);

		fwrite(m->texture.c_str(), m->texture.length(), 1, fp);
		fwrite(&ch, 1, 1, fp);

		fwrite(&m->ambient[0], sizeof(float), 1, fp);
		fwrite(&m->ambient[1], sizeof(float), 1, fp);
		fwrite(&m->ambient[2], sizeof(float), 1, fp);

		fwrite(&m->diffuse[0], sizeof(float), 1, fp);
		fwrite(&m->diffuse[1], sizeof(float), 1, fp);
		fwrite(&m->diffuse[2], sizeof(float), 1, fp);

		fwrite(&m->specular[0], sizeof(float), 1, fp);
		fwrite(&m->specular[1], sizeof(float), 1, fp);
		fwrite(&m->specular[2], sizeof(float), 1, fp);

		fwrite(&m->emissive[0], sizeof(float), 1, fp);
		fwrite(&m->emissive[1], sizeof(float), 1, fp);
		fwrite(&m->emissive[2], sizeof(float), 1, fp);

		fwrite(&m->shininess, sizeof(float), 1, fp);

		fwrite(&m->opacity, sizeof(float), 1, fp);
	}
}

void WriteTriangles(const std::vector<MeshTriangle> &triangles, FILE *fp)
{
	uint32_t count = triangles.size();
	if (count == 0)
		return;

	uint32_t size = 4; // count
	size += 
		(
			(4 * 3)    // vertex indices
			+ 4        // material index
		) * count;

	fputs("TRI", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);

	for (uint32_t i = 0; i < count; ++i)
	{
		const MeshTriangle *t = &triangles[i];

		fwrite(&t->vertices[0], 4, 1, fp);
		fwrite(&t->vertices[1], 4, 1, fp);
		fwrite(&t->vertices[2], 4, 1, fp);

		fwrite(&t->subMesh, 4, 1, fp);
	}
}

void WriteSubMeshes(const std::vector<SubMesh> &subMeshes, FILE *fp)
{
	uint32_t count = subMeshes.size();
	if (count == 0)
		return;

	uint32_t size = 4 +  // count
		4 +              // material index
		4;               // count of triangles

	// add up all the variable length string sizes for the submesh names
	for (uint32_t i = 0; i < count; ++i)
		size += subMeshes[i].name.length() + 1;    // include null terminator

	fputs("GRP", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);
	for (uint32_t i = 0; i < count; ++i)
	{
		const SubMesh *m = &subMeshes[i];

		fwrite(m->name.c_str(), m->name.length(), 1, fp);
		char c = '\0';
		fwrite(&c, 1, 1, fp);

		fwrite(&m->material, 4, 1, fp);
		fwrite(&m->numTriangles, 4, 1, fp);
	}
}

void WriteJoints(const std::vector<MeshJoint> &joints, FILE *fp)
{
	uint32_t count = joints.size();
	if (count == 0)
		return;

	uint32_t size = 4 +             // count
		(
			4 +                     // parent joint index
			(sizeof(float) * 3) +   // position (x, y, z)
			(sizeof(float) * 4)     // rotation (x, y, z, w)
		) * count;

	// add up all the variable length joint names
	for (uint32_t i = 0; i < count; ++i)
		size += joints[i].name.length() + 1;       // include null terminator

	fputs("JNT", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);
	for (uint32_t i = 0; i < count; ++i)
	{
		const MeshJoint *j = &joints[i];

		fwrite(j->name.c_str(), j->name.length(), 1, fp);
		char c = '\0';
		fwrite(&c, 1, 1, fp);

		int32_t parentIndex = GetIndexOf(joints, j->parentName);
		fwrite(&parentIndex, 4, 1, fp);

		fwrite(&j->localPosition.x, sizeof(float), 1, fp);
		fwrite(&j->localPosition.y, sizeof(float), 1, fp);
		fwrite(&j->localPosition.z, sizeof(float), 1, fp);

		fwrite(&j->localRotation.x, sizeof(float), 1, fp);
		fwrite(&j->localRotation.y, sizeof(float), 1, fp);
		fwrite(&j->localRotation.z, sizeof(float), 1, fp);
		fwrite(&j->localRotation.w, sizeof(float), 1, fp);
	}
}

void WriteJointToVertexMap(const std::vector<uint32_t> &vertexToJointMap, FILE *fp)
{
	uint32_t count = vertexToJointMap.size();
	if (count == 0)
		return;

	uint32_t size = 4 +      // count
		(4 * count);         // num vertices

	fputs("JTV", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&count, 4, 1, fp);
	for (uint32_t i = 0; i < count; ++i)
	{
		uint32_t jointIndex = vertexToJointMap[i];
		fwrite(&jointIndex, 4, 1, fp);
	}
}

void WriteJointKeyFrames(const std::vector<JointKeyFrames> &jointKeyFrames, FILE *fp)
{
	uint32_t numJoints = jointKeyFrames.size();
	if (numJoints == 0)
		return;

	uint32_t numFrames = jointKeyFrames[0].size();

	uint32_t size = 4 +                 // num frames
		(
			(
				(sizeof(float) * 3) +   // position (x, y, z)
				(sizeof(float) * 4)     // rotation (x, y, z, w)
				) * numJoints           // for each joint
		) * numFrames;                  // for each frame

	fputs("JKF", fp);
	fwrite(&size, 4, 1, fp);
	fwrite(&numFrames, 4, 1, fp);
	for (uint32_t i = 0; i < numJoints; ++i)
	{
		for (uint32_t j = 0; j < numFrames; ++j)
		{
			const MeshJointKeyFrame *frame = &jointKeyFrames[i][j];

			fwrite(&frame->position.x, sizeof(float), 1, fp);
			fwrite(&frame->position.y, sizeof(float), 1, fp);
			fwrite(&frame->position.z, sizeof(float), 1, fp);

			fwrite(&frame->rotation.x, sizeof(float), 1, fp);
			fwrite(&frame->rotation.y, sizeof(float), 1, fp);
			fwrite(&frame->rotation.z, sizeof(float), 1, fp);
			fwrite(&frame->rotation.w, sizeof(float), 1, fp);
		}
	}
}


int32_t GetIndexOf(const std::vector<MeshJoint> &joints, const std::string &name)
{
	for (uint32_t i = 0; i < joints.size(); ++i)
	{
		if (joints[i].name == name)
			return i;
	}

	return -1;
}
