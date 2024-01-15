#include "Enemy.h"

void Enemy::Initialize(const ModelData enemyData, const ModelData bulletData,Vector3 pos)
{
	enemyData_ = enemyData;
	bulletData_ = bulletData;

	
	enemy = enemy->Create(enemyData);
	enemy->worldTransform_.translation_ = pos;

	state = new EnemyStateApproach();
	state->Initialize(this);

}

void Enemy::Update()
{
	enemy->Update();

	state->Update(this);
}

void Enemy::Draw(ICamera* camera)
{
	enemy->Draw(camera);
}

void Enemy::Move(Vector3& velocity)
{
	SumVector3(enemy->worldTransform_.translation_, velocity);
}

void Enemy::ChangeState(IEnemyState* newState)
{
	state = newState;
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemy->worldTransform_.matWorld_.m[3][0];
	worldpos.y = enemy->worldTransform_.matWorld_.m[3][1];
	worldpos.z = enemy->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}
