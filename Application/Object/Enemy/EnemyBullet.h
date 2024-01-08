#pragma once

#include"ImGuiManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"

 
class EnemyBullet
{
public:

	~EnemyBullet();
	void Initialize(ICamera* camera, ModelData modelData, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw();

	void OnCollision();

	Vector3 GetWorldPosition();

	bool isDead()const { return isDead_; };

	
private:

	ICamera* camera_;

	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	Model* enemyBullet_ = nullptr;

};

