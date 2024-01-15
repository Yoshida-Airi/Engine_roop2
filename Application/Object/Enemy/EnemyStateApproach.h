#pragma once
#include"IEnemyState.h"
class EnemyStateApproach : public IEnemyState
{
public:
	void Initialize(Enemy* pEnmey);
	void Update(Enemy* pEnemy);
};