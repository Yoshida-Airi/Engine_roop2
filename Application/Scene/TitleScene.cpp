#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete uiCamera;
	delete sprite;
}

void TitleScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();


	uvTexture = texture->LoadTexture("Resources/uvChecker.png");


	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	sprite = new Sprite;
	sprite->Initialize(uvTexture);
	

}

void TitleScene::Update()
{
	camera->CameraDebug();
	sprite->SpriteDebug();

	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	sprite->Update();


	
}

void TitleScene::Draw()
{
	sprite->Draw(uiCamera);
}

