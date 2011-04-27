#include "utils.h"

#include <stdio.h>

std::string GetMeshFilenameFromInput(const std::string &filename)
{
	std::string meshFilename = "";

	// strip off the existing extension, and replace it with ".mesh"
	size_t extStart = filename.find_last_of('.');
	if (extStart != std::string::npos)
	{
		meshFilename = filename.substr(0, extStart);
		meshFilename += ".mesh";
	}

	return meshFilename;
}
