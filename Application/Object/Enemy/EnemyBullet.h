#pragma once
#include"Model.h"
#include"Collider.h"

class Player;
class EnemyBullet :public Collider
{
public:
	void Initialize( Vector3 pos, const Vector3& velocity);
	void Update();
	void Draw(ICamera* camera);

	Vector3 GetWorldPosition()override;

	/// <summary>
	/// 弾のデスフラグが立っているか
	/// </summary>
	/// <returns>true : 立っている</returns>
	bool IsDead()const { return isDead_; };

	//セッター
	void SetPlayer(Player* player) { player_ = player; };

	void OnCollision()override;

private:

	std::unique_ptr<Model> bulletModel_ = nullptr;

	//速度
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	Player* player_ = nullptr;
};

