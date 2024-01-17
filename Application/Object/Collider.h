#pragma once
#include"VectorMath.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
public:

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	//衝突時に呼ばれる関数
	virtual void OnCollision() {};

	//半径を取得
	float GetRadius() const { return radius_; };

	//半径を設定
	void SetRadius(float radius) { radius_ = radius; };

private:
	//衝突半径
	float radius_ = 2.0f;


};