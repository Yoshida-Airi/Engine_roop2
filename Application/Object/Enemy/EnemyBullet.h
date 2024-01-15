#pragma once
#include"Model.h"

class Player;
class EnemyBullet
{
public:
	void Initialize(ModelData bulletData, Vector3 pos, const Vector3& velocity);
	void Update();
	void Draw(ICamera* camera);

	Vector3 GetWorldPosition();

	/// <summary>
	/// 弾のですフラグが立っているか
	/// </summary>
	/// <returns>true : 立っている</returns>
	bool IsDead()const { return isDead_; };

	void SetPlayer(Player* player) { player_ = player; };

private:

	std::unique_ptr<Model> bullet = nullptr;

	//速度
	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	Player* player_ = nullptr;
};

