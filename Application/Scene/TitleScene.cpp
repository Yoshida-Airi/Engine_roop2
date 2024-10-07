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

	titleTexture = textureManager->LoadTexture("Resources/Scene/title.png");

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/SampleSound/Alarm01.wav");
	//Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();

	title.reset(Sprite::Create(titleTexture));
	title->SetisInvisible(true);


	fence_.reset(Model::Create("Resources/SampleAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	fence_->GetWorldTransform()->rotation_.y = 3.1f;

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

	if (input->TriggerKey(DIK_SPACE))
	{
		fade_->Start(Fade::Status::FadeOut, 1.5f);
		//Audio::GetInstance()->SoundStopWave(soundData);
	}

	if (fade_->IsFinished())
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	title->Update();

	fence_->Update();
	cube_->Update();


	
	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

	titleEffect_->Update();
	
}

void TitleScene::Draw()
{
	title->Draw(camera);

	fence_->Draw(camera);
	cube_->Draw(camera);

	titleEffect_->Draw();
	

	fade_->Draw(camera);

	
}

