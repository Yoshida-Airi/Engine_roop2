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

	//衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; };
	//衝突属性(自分)を指定
	void SetCollisionAttribute(uint32_t attribute)
	{
		collisionAttribute_ = attribute;
	}

	//衝突マスク(相手)を取得
	uint32_t GetCollisionMask() const { return CollisionMask_; };
	
	//衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t mask)
	{
		CollisionMask_ = mask;
	}

private:
	//衝突半径
	float radius_ = 2.0f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
};