/**
*	@file Transform.h
*	@brief トランスフォーム構造体クラスヘッダ
*/


#pragma once
#include"VectorMath.h"

struct EulerTransform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};