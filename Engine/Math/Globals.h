#pragma once
#pragma once
#include"VectorMath.h"
#include"MatrixMath.h"

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material
{
	Vector4 color;
	Matrix4x4 uvTransform;
	int32_t enableLighting;
	float shininess;
	float padding[2];
	
};

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;
};
