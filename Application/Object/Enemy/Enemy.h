#pragma once
#include"Model.h"
#include"IEnemyState.h"
#include"EnemyStateApproach.h"

class Enemy
{
public:
	void Initialize(const ModelData enemyData, const ModelData bulletData, Vector3 pos);
	void Update();
	void Draw(ICamera* camera);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="velocity">移動する速度</param>
	void Move(Vector3& velocity);

	// フェーズの以降
	void ChangeState(IEnemyState* newState);

	//ゲッター
	Vector3 GetWorldPosition();


private:

	std::unique_ptr<Model>enemy = nullptr;
	ModelData enemyData_;

	////弾
	//std::list<EnemyBullet*> bullets_;
	ModelData bulletData_;

	// ステート
	IEnemyState* state;

private:


};

