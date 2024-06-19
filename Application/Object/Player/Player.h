#pragma once
#include"Input.h"

#include"Camera.h"
#include"Model.h"
#include"VectorMath.h"

#include"MathUtilty.h"

#include<algorithm>
#include<numbers>



class Player
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);
	WorldTransform* GetWorldTransform() { return playerModel->GetWorldTransform(); }

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

private:

	/// <summary>
	///　移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 振り向き
	/// </summary>
	void Turn();



};



