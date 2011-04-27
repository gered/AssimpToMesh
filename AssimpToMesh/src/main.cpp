#include <stdio.h>
#include <string>
#include <exception>

#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#include "utils/utils.h"
#include "assimputils/utils.h"

int main(int argc, char *argv[])
{
	printf("ASSIMP-to-MESH Converter.\n");

	if (argc == 1)
	{
		printf("Usage: assimptomesh.exe [inputfile]\n\n");
		return 0;
	}

	std::string file = argv[1];
	std::string outputFile = GetMeshFilenameFromInput(file);

	if (outputFile.length() == 0)
	{
		printf("Unable to determine output mesh filename from the input file name.\n\n");
		return 1;
	}

	printf("Input file: %s\n", file.c_str());
	printf("Output file: %s\n", outputFile.c_str());

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(file,
		aiProcess_JoinIdenticalVertices | 
		aiProcess_Triangulate | 
		aiProcess_RemoveRedundantMaterials | 
		aiProcess_SortByPType | 
		aiProcess_OptimizeMeshes | 
		aiProcess_OptimizeGraph
		);

	if (!scene)
	{
		printf("Failed to load input file: %s\n\n", importer.GetErrorString());
		return 1;
	}

	printf("Scene loaded.\n");
	printf("Animated? %d\nSkeletal? %d\nStatic? %d\n", IsSceneAnimated(scene), IsSceneSkeletal(scene), IsSceneStatic(scene));

	return 0;
}
