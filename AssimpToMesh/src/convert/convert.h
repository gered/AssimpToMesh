#ifndef __CONVERT_CONVERT_H_INCLUDED__
#define __CONVERT_CONVERT_H_INCLUDED__

#include <string>
#include <aiScene.h>

void ConvertStatic(const std::string &outfile, const aiScene *scene);
void ConvertSkeletalAnimated(const std::string &outfile, const aiScene *scene);

#endif
