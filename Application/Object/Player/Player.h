#pragma once
#define NOMINMAX

#include"Input.h"

#include"Camera.h"
#include"Model.h"
#include"VectorMath.h"

#include"MathUtilty.h"

#include<algorithm>
#include<numbers>

#include"GameObject.h"

class Player :public GameObject
{
public:
	void Initialize(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(Camera* camera)override;
	WorldTransform* GetWorldTransform() { return playerModel->GetWorldTransform(); }

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	
	Model*playerModel;
	Vector3 velocity_ = {  };

	static inline const float kAcceleration = 0.01f;	//加速度
	static inline const float kAttenuation = 0.03f;	//速度減衰率
	static inline const float kLimitRunSpeed = 0.1f;	//最大速度

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
	static inline const float kGravityAcceleration = 9.8f;	//重力加速度（下方向
	static inline const float kLimitFallSpead = 0.1f;		//最大落下速度（下方向
	static inline const float kJumpAcceleration = 7.0f;	//ジャンプ初速（上方向
	static inline const float kLimitJumpSpead = 5.1f;		//最大落下速度（下方向
	

	static inline const float kDeltaTime = 1.0f / 60.0f;      // 1フレームの時間（60FPSと仮定）


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




};



