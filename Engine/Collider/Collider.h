#pragma once
#include"VectorMath.h"
#include"WorldTransform.h"
#include"Model.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider
{
public:

	void Initialize();
	void UpdateWorldTransform();
	void Draw(Camera* camera);

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) {};

	//半径を取得
	Vector3 GetRadius() const { return radius_; };

	//半径を設定
	void SetRadius(Vector3 radius) { radius_ = radius; };

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

	//種別IDを取得
	uint32_t GetTypeID()const { return typeID_; };
	//種別IDを設定
	void SetTypeID(uint32_t typeID) { typeID_ = typeID; };

	//コライダータイプIDを取得
	uint32_t GetColliderTypeID()const { return collisionType_; };
	//コライダータイプIDを設定
	void SetColliderTypeID(uint32_t collisionType) { collisionType_ = collisionType; };


private:
	//衝突半径
	Vector3 radius_ = { 1.0f,1.0f,1.0f };
	uint32_t typeID_ = 0u;
	uint32_t collisionType_ = 0u;


	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
	WorldTransform* worldTransform;

	std::unique_ptr<Model>model_;

};