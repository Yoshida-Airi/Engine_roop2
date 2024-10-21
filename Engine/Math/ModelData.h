/**
*	@file ModelData.h
*	@brief モデルデータ構造体用クラスヘッダ
*/


#pragma once
#include<string>
#include<vector>
#include"Globals.h"
#include"Transform.h"
#include<map>

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

struct VertexWeightData
{
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData
{
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData>vertexWeights;
};

struct ModelData
{
	std::vector<VertexData>vertices;
	std::vector<uint32_t>indices;
	MaterialData material;
	std::string filename{};
	Node rootNode;
	std::map<std::string, JointWeightData> skinClusterData;
};



