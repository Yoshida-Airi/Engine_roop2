#include "EnemyStateApproach.h"
#include"EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateApproach::Initialize(Enemy* pEnmey) { pEnmey->GetWorldPosition(); }
void EnemyStateApproach::Update(Enemy* pEnemy)
{
	// 速度
	Vector3 velocity;
	velocity = { 0, 0, -0.3f };
	pEnemy->Move(velocity);
	if (pEnemy->GetWorldPosition().z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}