#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Model.h"
#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"
#include"collisionManager.h"

#include"Input.h"

#include"Player.h"
#include"Enemy.h"
#include"Skydome.h"
#include"RailCamera.h"

#include<sstream>

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene:public BaseScene
{
public:
	~GamePlayScene()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵の発生
	/// </summary>
	void SpawnEnemy(const Vector3& position);


	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// <summary>
	void UpdateEnemyPopCommands();


	void StartFadeOut();

	void UpdateFadeOut();


private:
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;

	//当たり判定処理
	CollisionManager* colliderManager_ = nullptr;

	ICamera* camera;
	ICamera* uiCamera;
	RailCamera* railCamera = nullptr;

	Input* input;


	Player* player = nullptr;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;
	bool isWaitTime_ = true;	//待機中フラグ　true::待機中
	int32_t waitTimer_ = 0;	//待機タイマー
	std::list<Enemy*> enemys;
	std::list<EnemyBullet*> enemyBullets_;

	Skydome* skydome = nullptr;

	uint32_t fadeTex;
	std::unique_ptr<Sprite>fadeSprite = nullptr;

	float alpha = 0;
	bool isFadingOut = false;

	float clearTIme = 0;

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

	//ワールドトランスフォーム
	WorldTransform railCameraWorldTransform_;


};

