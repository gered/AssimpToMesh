#ifndef __ASSIMPUTILS_UTILS_H_INCLUDED__
#define __ASSIMPUTILS_UTILS_H_INCLUDED__

#include <assimp.hpp>
#include <aiScene.h>

bool IsSceneAnimated(const aiScene *scene);
bool IsSceneSkeletal(const aiScene *scene);
bool IsSceneStatic(const aiScene *scene);

#endif
