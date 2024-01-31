#include "GameClearScene.h"

GameClearScene::~GameClearScene()
{
	delete camera;
	delete uiCamera;
}

void GameClearScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();


	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	overTex = TextureManager::GetInstance()->LoadTexture("Resources/Clear.png");
	overSprite.reset(Sprite::Create(overTex));
}

void GameClearScene::Update()
{
	camera->UpdateMatrix();
	uiCamera->UpdateMatrix();


	//シーン遷移
	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("TITLE");
		}

	}

	overSprite->Update();
}

void GameClearScene::Draw()
{
	overSprite->Draw(uiCamera);
}
