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
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	texture = TextureManager::GetInstance();

	uvTexture = texture->LoadTexture("Resources/Title.png");


	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	sprite = new Sprite;
	sprite->Initialize(uvTexture);
	

}

void TitleScene::Update()
{
#ifdef _DEBUG
	camera->CameraDebug();
#endif // _DEBUG

	

	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
		}
	}

	sprite->Update();

}

void TitleScene::Draw()
{
	sprite->Draw(uiCamera);
}

