#include "utils.h"

bool IsSceneAnimated(const aiScene *scene)
{
	return scene->HasAnimations();
}

bool IsSceneSkeletal(const aiScene *scene)
{
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		if (scene->mMeshes[i]->HasBones())
			return true;
	}

	return false;
}

bool IsSceneStatic(const aiScene *scene)
{
	if (IsSceneAnimated(scene) || IsSceneSkeletal(scene))
		return false;
	else
		return true;
}
