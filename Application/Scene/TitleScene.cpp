#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete efect;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	textureManager = TextureManager::GetInstance();

	titleLogo = textureManager->LoadTexture("Resources/Scene/logo.png");
	backGroundTexture= textureManager->LoadTexture("Resources/Scene/backGround.png");
	spaceTexture = textureManager->LoadTexture("Resources/Scene/space.png");

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();

	title.reset(Sprite::Create(titleLogo));
	title->GetWorldTransform()->translation_ = { 220.0f,110.0f };

	backGround.reset(Sprite::Create(backGroundTexture));

	space.reset(Sprite::Create(spaceTexture));
	space->GetWorldTransform()->translation_ = { 435.0f,490.0f };
	

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();
	skydome->SetLight(false);


	fade_ = std::make_unique <Fade>();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.5f);

	titleEffect_ = std::make_unique <TitleEffect>();
	titleEffect_->Initialize(camera);
	titleEffect_->SetFlag(true);
	titleEffect_->SetPosition({ 0.0f,-3.5f,0.0f });


}

void TitleScene::Update()
{
	camera->CameraDebug();

	fade_->Update();

	UpdateSpriteBlink();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
			//Audio::GetInstance()->SoundStopWave(soundData);
		}
	}

	if (input->TriggerKey(DIK_SPACE) && isSceneChange == false)
	{
		fade_->Start(Fade::Status::FadeOut, 1.5f);
		isSceneChange = true;
		//Audio::GetInstance()->SoundStopWave(soundData);
	}

	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	title->Update();
	backGround->Update();
	space->Update();

	skydome->Update();
	
	

	titleEffect_->Update();

	
	
}

void TitleScene::Draw()
{
	

	
	
	skydome->Draw(camera);

	titleEffect_->Draw();
	//backGround->Draw(camera);
	title->Draw(camera);
	space->Draw(camera);

	fade_->Draw(camera);

	
}

void TitleScene::UpdateSpriteBlink()
{
	frameCount++;

	if (frameCount >= blinkFrames)
	{
		space->SetisInvisible(!space->GetisInvisible());
		frameCount = 0;
	}
}