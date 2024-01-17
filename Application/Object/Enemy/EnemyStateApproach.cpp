#include "EnemyStateApproach.h"
#include"EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateApproach::Initialize(Enemy* pEnemy)
{
	pEnemy->GetWorldPosition();
	pEnemy->AttackReset();
}

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	
	// 速度
	Vector3 velocity;
	velocity = { 0, 0, -0.0f };
	pEnemy->Move(velocity);
	if (pEnemy->GetWorldPosition().z < 0.0f) 
	{
		pEnemy->DeleteBullet();
		pEnemy->ChangeState(new EnemyStateLeave());
	}

	

}