#include "Enemy.h"

void Enemy::Initialize(const ModelData enemyData, const ModelData bulletData)
{
	enemyData_ = enemyData;
	bulletData_ = bulletData;

	enemy = enemy->Create(enemyData);

}

void Enemy::Update()
{
	enemy->Update();

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,-0.1f };
	// 移動
	SumVector3(enemy->worldTransform_.translation_, move);
}

void Enemy::Draw(ICamera* camera)
{
	enemy->Draw(camera);
}
