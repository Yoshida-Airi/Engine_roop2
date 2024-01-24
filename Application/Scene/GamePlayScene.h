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
	std::list<Enemy*> enemys;
	std::list<EnemyBullet*> enemyBullets_;

	Skydome* skydome = nullptr;



#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG

	//ワールドトランスフォーム
	WorldTransform railCameraWorldTransform_;

};

