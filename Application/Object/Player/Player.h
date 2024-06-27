#pragma once
#define NOMINMAX

#include"Input.h"

#include"Camera.h"
#include"Model.h"
#include"VectorMath.h"

#include"MathUtilty.h"

#include<algorithm>
#include<numbers>

#include"Collider.h"

class Player : public Collider
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);
	WorldTransform* GetWorldTransform() { return playerModel->GetWorldTransform(); }

	Vector3 GetWorldPosition()override;
	void OnCollision([[maybe_unused]] Collider* other)override;


private:
	std::unique_ptr<Model>playerModel;
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
	static inline const float kLimitFallSpead = 0.3f;		//最大落下速度（下方向
	static inline const float kJumpAcceleration = 7.0f;	//ジャンプ初速（上方向
	

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



