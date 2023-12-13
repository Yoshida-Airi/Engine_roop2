#pragma once
#include<string>
#include<vector>
#include"Globals.h"

struct MaterialData
{
	std::string textureFilePath;
};

struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
	std::string filename{};
};