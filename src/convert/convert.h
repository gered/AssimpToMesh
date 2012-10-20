#ifndef __CONVERT_CONVERT_H_INCLUDED__
#define __CONVERT_CONVERT_H_INCLUDED__

#include <string>
#include <assimp/aiScene.h>

void ConvertStatic(const std::string &outfile, const aiScene *scene, float scaleFactor);
void ConvertSkeletalAnimated(const std::string &outfile, const aiScene *scene, float scaleFactor);

#endif
