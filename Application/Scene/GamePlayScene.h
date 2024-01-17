#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Sphere.h"
#include"Model.h"
#include"ICamera.h"
#include"Camera.h"
#include"UICamera.h"

#include"Input.h"

#include"Player.h"
#include"Enemy.h"

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

private:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheackAllCollisions();

private:
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;

	ModelData plane;
	ModelData cube;

	ICamera* camera;
	ICamera* uiCamera;

	Input* input;


	std::unique_ptr<Triangle> triangle = nullptr;
	std::unique_ptr<Triangle> triangle2 = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;
	std::unique_ptr<Sprite> sprite2 = nullptr;
	std::unique_ptr<Sphere> sphere = nullptr;


	std::unique_ptr<Model> playerModel = nullptr;
	std::unique_ptr<Model> PlayerBulletModel = nullptr;
	std::unique_ptr<Model> enemyModel = nullptr;
	std::unique_ptr<Model> enemyBulletModel = nullptr;

	Player* player = nullptr;
	Enemy* enemy = nullptr;

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG


};

