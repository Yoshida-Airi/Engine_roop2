#pragma once
#pragma once
#include"VectorMath.h"
#include"MatrixMath.h"


//シェーダーに渡す
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
	float shininess;
	int32_t enableLighting;
	int padding[2];
};

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;
};


//スプライト用


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