#pragma once
#include"Camera.h"
#include"GlobalVariables.h"

//前方宣言
class Player;

class CameraController
{
public:
	~CameraController();
	void Initialize(Camera* camera);
	void Update();

	//セッター
	void SetTarget(Player* target)
	{
		target_ = target;
	}

	void Reset();

	void ApplyGlobalVariables();

private:
	Camera* camera_;
	const char* groupName;
	Player* target_ = nullptr;
	GlobalVariables* globalVariables = nullptr;

	//追従対象とカメラの座標の差
	Vector3 targetOffset_ = { 0,3.0f,-30.0f };
	//カメラの目標座標
	Vector3 targetPosition = {};
	//座標補間割合
	float interpolationRate = 0.04f;
	


};

