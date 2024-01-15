#include "EnemyStateLeave.h"
#include"Enemy.h"

void EnemyStateLeave::Initialize(Enemy* pEnemy) { pEnemy->GetWorldPosition(); }

void EnemyStateLeave::Update(Enemy* pEnemy) {

	Vector3 velocity;
	velocity = { -0.5f, 0.5f, 0 };
	pEnemy->Move(velocity);
}
