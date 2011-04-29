#ifndef __CONVERT_MESHJOINT_H_INCLUDED__
#define __CONVERT_MESHJOINT_H_INCLUDED__

#include <stdint.h>
#include <string>
#include <aiVector3D.h>
#include <aiQuaternion.h>
#include <vector>

struct MeshJoint
{
	std::string name;
	std::string parentName;
	aiVector3D position;
	aiQuaternion rotation;
};

struct MeshJointKeyFrame
{
	aiVector3D position;
	aiQuaternion rotation;
};

typedef std::vector<MeshJointKeyFrame> JointKeyFrames;

#endif
