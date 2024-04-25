#pragma once
#include<string>
#include<vector>
#include"Globals.h"
#include"Transform.h"

struct MaterialData
{
	std::string textureFilePath;
};

struct Node
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};

struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
	std::string filename{};
	Node rootNode;
};

