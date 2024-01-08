#pragma once
#include"ImGuiManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"

#include"Input.h"


class PlayerBullet
{
public:

	~PlayerBullet();
	void Initialize(ICamera* camera, ModelData modelData, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw();

	bool isDead()const { return isDead_; };

private:

	Input* input;
	ICamera* camera_;

	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	Model* playerBullet_ = nullptr;
};

