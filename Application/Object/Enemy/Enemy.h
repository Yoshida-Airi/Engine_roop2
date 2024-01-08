#pragma once
#include"ImGuiManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"

#include"EnemyBullet.h"

class Player;
class Enemy
{
public:
	~Enemy();
	void Initialize(ICamera* camera, ModelData model,ModelData bullet, const Vector3& position, const Vector3& velocityA, const Vector3& velocityB);
	void Update();
	void Draw();

	enum class Phase
	{
		Approach,
		Leave,
	};

	void SetPlayer(Player* player) { player_ = player; };

	Vector3 GetWorldPosition();
private:

	//自キャラ
	Player* player_ = nullptr;

	//発射間隔
	static const int kFireInterval = 20;

	void Approach();
	void Leave();
	void Fire();

	ICamera* camera_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	ModelData enemyData_;
	ModelData bulletData_;
	// モデル
	Model* enemy_ = nullptr;

	// 速度
	Vector3 velocity_;

	Phase phase_ = {};
	// 速度
	Vector3 ApprochVelocity_;
	Vector3 LeaveVelocity_;

	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t timer = 0;
};

