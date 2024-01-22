#pragma once
#include"VectorMath.h"
#include"MatrixMath.h"

struct SpriteData
{
	Vector4 vertex[4];
	Vector4 color;
};

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};

enum VertexNumber
{
	LB,
	LT,
	RB,
	RT
};