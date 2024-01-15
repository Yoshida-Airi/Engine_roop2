#pragma once

class Enemy;//前方宣言
class IEnemyState
{
public:
	// 仮想デストラクタ
	virtual ~IEnemyState() {};

	// 純粋仮想関数
	virtual void Initialize(Enemy* pEnemy) = 0;
	virtual void Update(Enemy* pEnemy) = 0;

};