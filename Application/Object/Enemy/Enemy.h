#pragma once
#include"Model.h"
#include"IEnemyState.h"
#include"EnemyStateApproach.h"

#include"EnemyBullet.h"
#include"TimedCall.h"

#include "Collider.h"

#include<list>
class GamePlayScene;
class Player;
class Enemy :public Collider
{
public:
	~Enemy();
	void Initialize( Vector3 pos);
	void Update();
	void Draw(ICamera* camera);

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="velocity">移動する速度</param>
	void Move(Vector3& velocity);

	// フェーズの以降
	void ChangeState(IEnemyState* newState);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Fire();

	//ゲッター
	Vector3 GetWorldPosition()override;

	//発射間隔
	static const int kFireInterval = 45;

	//弾を発射し、タイマーをリセットするコールバック関数
	void AttackReset();

	void OnCollision()override;

	/// <summary>
	/// タイムコールをすべて消す
	/// </summary>
	void DeleteBullet();

	//セッター
	void SetPlayer(Player* player) { player_ = player; };

	
	void SetGameScene(GamePlayScene* gameScene)
	{
		gameScene_ = gameScene;
	}

	bool IsDead() const { return isDead_; };

private:

	std::unique_ptr<Model>  enemyModel_ = nullptr;

	// ゲームシーン
	GamePlayScene* gameScene_ = nullptr;

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	// ステート
	IEnemyState* state;

	Player* player_ = nullptr;

	bool isDead_ = false;

};

