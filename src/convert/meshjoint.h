#ifndef __CONVERT_MESHJOINT_H_INCLUDED__
#define __CONVERT_MESHJOINT_H_INCLUDED__

#include <stdint.h>
#include <string>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <vector>

struct MeshJoint
{
	std::string name;
	std::string parentName;
	aiVector3D localPosition;
	aiQuaternion localRotation;
	aiVector3D offsetPosition;
	aiQuaternion offsetRotation;
};

struct MeshJointKeyFrame
{
	aiVector3D position;
	aiQuaternion rotation;
};

typedef std::vector<MeshJointKeyFrame> JointKeyFrames;

int32_t GetIndexOf(const std::vector<MeshJoint> &joints, const std::string &name);

#endif
