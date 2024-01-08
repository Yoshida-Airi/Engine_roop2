#pragma once

#include"BaseScene.h"

#include"ImGuiManager.h"
#include"SceneFactory.h"
#include"SceneManager.h"

//#include"Triangle.h"
//#include"Sprite.h"
//#include"Sphere.h"
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
	TextureManager* texture;
	SceneManager* sceneManager_ = nullptr;

	uint32_t uvTexture;
	uint32_t monsterBall;
	uint32_t Doll;

	ModelLoader* object;
	ModelData plane;
	ModelData cube;

	ICamera* camera;
	ICamera* uiCamera;

	Input* input;


	//Triangle* triangle = nullptr;
	//Triangle* triangle2 = nullptr;
	//Sprite* sprite = nullptr;
	//Sprite* sprite2 = nullptr;
	//Sphere* sphere = nullptr;
	//Model* model = nullptr;
	//Model* model2 = nullptr;

	ModelData playerData;
	ModelData playerBulletData;
	ModelData EnemyData;
	ModelData EnemyBulletData;
	Player* player = nullptr;
	Enemy* enemy = nullptr;

#ifdef _DEBUG

	ImGuiManager* imgui;

#endif // _DEBUG


};

