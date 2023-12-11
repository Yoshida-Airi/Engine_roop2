#pragma once
#pragma once
#include"VectorMath.h"
#include"MatrixMath.h"

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
};

struct Material
{
	Vector4 color;
	Matrix4x4 uvTransform;
};