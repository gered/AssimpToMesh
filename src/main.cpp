#include <stdio.h>
#include <string>
#include <stdexcept>

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include "utils/utils.h"
#include "assimputils/utils.h"
#include "convert/convert.h"
#include "nodetree/nodetree.h"

int main(int argc, char *argv[])
{
	printf("ASSIMP-to-MESH Converter.\n");

	if (argc == 1)
	{
		printf("Usage: assimptomesh.exe [--describe=txtfile] [--out=outputfile] [--scale=scalefactor] inputfile\n\n");
		return 0;
	}

	// input file is always the last argument
	std::string file = argv[argc - 1];

	// default option values
	float scaleFactor = 1.0f;
	std::string outputFile = "";
	std::string describeFile = "";

	// find any options
	for (int i = 1; i < argc - 1; ++i)
	{
		std::string arg = argv[i];

		if (arg.substr(0, 11) == "--describe=")
		{
			// description file

			if (arg.length() == 11)
			{
				printf("Missing description output filename.\n");
				return 1;
			}
			describeFile = arg.substr(11);
		}

		else if (arg.substr(0, 6) == "--out=")
		{
			// different output filename

			if (arg.length() == 6)
			{
				printf("Missing output filename.\n");
				return 1;
			}
			outputFile = arg.substr(6);
		}

		if (arg.substr(0, 8) == "--scale=")
		{
			// scale factor

			if (arg.length() == 8)
			{
				printf("Missing scale factor.\n");
				return 1;
			}

			scaleFactor = (float)atof(arg.substr(8).c_str());

			if (scaleFactor == 0.0f)
			{
				printf("Invalid or 0.0 scale factor.\n");
				return 1;
			}
		}
	}

	// attempt to load the input file
	printf("Input file: %s\n", file.c_str());
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(file,
		aiProcess_JoinIdenticalVertices | 
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate | 
		aiProcess_RemoveRedundantMaterials | 
		aiProcess_SortByPType | 
		aiProcess_OptimizeMeshes | 
		aiProcess_OptimizeGraph | 
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_FlipUVs |
		aiProcess_FindInvalidData
		);

	if (!scene)
	{
		printf("Failed to load input file: %s\n\n", importer.GetErrorString());
		return 1;
	}
	printf("Scene loaded.\n");

	// input file loaded, now get the output filename (if not provided via command line)
	if (outputFile.length() == 0)
	{
		outputFile = GetMeshFilenameFromInput(file);
		if (outputFile.length() == 0)
		{
			printf("Unable to determine output mesh filename from the input file name.\n\n");
			return 1;
		}
	}
	printf("Output file: %s\n", outputFile.c_str());

	// optionally show info about the entire scene before doing the conversion
	if (describeFile.length() > 0)
	{
		try
		{
			Walk(scene, describeFile);
		}
		catch (std::exception &ex)
		{
			printf("Error: %s\n", ex.what());
			return 1;
		}
	}

	// attempt conversion using an appropriate converter based on what kind of scene it is
	if (IsSceneStatic(scene))
	{
		printf("Using static converter.\n");
		try
		{
			ConvertStatic(outputFile, scene, scaleFactor);
		}
		catch (std::exception &ex)
		{
			printf("Error: %s\n", ex.what());
			return 1;
		}
		printf("Convert complete.\n");
	}
	else if (IsSceneAnimated(scene) && IsSceneSkeletal(scene))
	{
		printf("Using skeletal animation converter.\n");
		try
		{
			ConvertSkeletalAnimated(outputFile, scene, scaleFactor);
		}
		catch (std::exception &ex)
		{
			printf("Error: %s\n", ex.what());
			return 1;
		}
		printf("Convert complete.\n");
	}
	else
	{
		printf("Unknown scene format.\n");
		return 1;
	}

	return 0;
}
