#pragma once
#define NOMINMAX


#include"Camera.h"
#include"Model.h"

#include"Input.h"
#include"VectorMath.h"
#include"MathUtilty.h"
#include"GlobalVariables.h"
#include"GameObject.h"


#include<algorithm>
#include<numbers>


class Weapon;
class Player :public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;
	WorldTransform* GetWorldTransform() { return playerModel->GetWorldTransform(); }

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

	void SetWeapon(Weapon* weapon)
	{
		weapon_ = weapon;
	}

private:

	GlobalVariables* grobalVariables;
	const char* groupName;

	std::unique_ptr<Model>playerModel;
	std::vector<Model*>playerModels;


	Vector3 velocity_ = {  };

	float kAcceleration = 0.01f;	//加速度
	float kAttenuation = 0.03f;	//速度減衰率
	float kLimitRunSpeed = 0.1f;	//最大速度

	//左右
	enum class LRDirection
	{
		kRight,
		kLeft,
	};


	//向いている方向
	LRDirection lrDirection = LRDirection::kRight;
	float turnFirstRotationY = 0.0f;	//旋回開始時の角度
	float turnTimer = 0.0f;				//旋回タイマー
	static inline const float kTimeTrun = 1.0f;	//旋回時間<秒>

	bool onGround_ = true;	//接地状態フラグ
	bool landing = false;	//接地フラグ
	float kGravityAcceleration = 0.05f;	//重力加速度（下方向
	float kLimitFallSpead = 0.5f;		//最大落下速度（下方向
	float kJumpAcceleration = 1.0f;	//ジャンプ初速（上方向

	Weapon* weapon_;

private:

	/// <summary>
	///　移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 振り向き
	/// </summary>
	void Turn();

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void ApplyGlobalVariables();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>
	void BehaviorAttackUpdate();
};



