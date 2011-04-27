#ifndef __CONVERT_MESHMATERIAL_H_INCLUDED__
#define __CONVERT_MESHMATERIAL_H_INCLUDED__

#include <string>
#include <aiMaterial.h>

struct MeshMaterial
{
	MeshMaterial()
	{
	}

	MeshMaterial(const aiMaterial *source)
	{
		aiString name;
		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		aiColor3D emissive;
		float shininess;
		float opacity;

		source->Get(AI_MATKEY_NAME, name);
		source->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		source->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		source->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		source->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
		source->Get(AI_MATKEY_SHININESS, shininess);
		source->Get(AI_MATKEY_OPACITY, opacity);

		this->name = std::string(name.data, name.length);

		this->ambient[0] = ambient.r;
		this->ambient[1] = ambient.g;
		this->ambient[2] = ambient.b;

		this->diffuse[0] = diffuse.r;
		this->diffuse[1] = diffuse.g;
		this->diffuse[2] = diffuse.b;

		this->specular[0] = specular.r;
		this->specular[1] = specular.g;
		this->specular[2] = specular.b;

		this->emissive[0] = emissive.r;
		this->emissive[1] = emissive.g;
		this->emissive[2] = emissive.b;

		this->shininess = shininess;

		this->opacity = opacity;
	}

	std::string name;
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emissive[3];
	float shininess;
	float opacity;
};

#endif
