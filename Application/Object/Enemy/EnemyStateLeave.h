#pragma once
#include"IEnemyState.h"


class EnemyStateLeave : public IEnemyState
{
public:
	void Initialize(Enemy* pEnmey);
	void Update(Enemy* pEnemy);
};
