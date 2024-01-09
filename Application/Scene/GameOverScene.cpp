#include "GameOverScene.h"

#include"SceneManager.h"

GameOverScene::~GameOverScene()
{
	delete camera;
	delete uiCamera;
	delete sprite;
}

void GameOverScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	texture = TextureManager::GetInstance();

	uvTexture = texture->LoadTexture("Resources/Over.png");


	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	sprite = new Sprite;
	sprite->Initialize(uvTexture);


}

void GameOverScene::Update()
{
	camera->CameraDebug();

	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("TITLE");
		}
	}

	sprite->Update();

}

void GameOverScene::Draw()
{
	sprite->Draw(uiCamera);
}

