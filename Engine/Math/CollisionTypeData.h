#pragma once

#include"VectorMath.h"

struct AABB
{
	Vector3 min;
	Vector3 max;
};

struct  SphereData
{
	Vector3 center;//中心点
	Vector3 radius; //半径
};
