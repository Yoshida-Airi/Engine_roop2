#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{
	//delete triangle;
	//delete triangle2;
	//delete sprite;
	//delete sprite2;
	//delete sphere;
	//delete model;
	//delete model2;


	delete camera;
	delete uiCamera;
	delete player;

}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

//
//#ifdef _DEBUG
//	imgui = ImGuiManager::GetInstance();
//#endif // _DEBUG


	object = ModelLoader::GetInstance();
	playerData = object->LoadObjFile("Resources/Player", "player.obj");
	playerBulletData = object->LoadObjFile("Resources/Player", "playerBullet.obj");


//	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
//	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
//	Doll = texture->LoadTexture("Resources/Doll.png");
//
//	object = ModelLoader::GetInstance();
//	plane = object->LoadObjFile("Resources", "plane.obj");
//	cube = object->LoadObjFile("Resources", "cube.obj");
//
	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	player = new Player();
	player->Initialize(camera, playerData,playerBulletData);

}

void GamePlayScene::Update()
{
	input->TriggerKey(DIK_0);

#ifdef _DEBUG
	
	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	player->Update();
}

void GamePlayScene::Draw()
{

	player->Draw();

}
