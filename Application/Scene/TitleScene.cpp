#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	texture = TextureManager::GetInstance();

	soundData = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	Audio::GetInstance()->SoundPlayWave(soundData, false);

	uvTexture = texture->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	circle = texture->LoadTexture("Resources/DefaultAssets/circle.png");

	camera = new Camera;
	camera->Initialize();


	fence_.reset(Model::Create("Resources/DefaultAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	fence_->GetWorldTransform()->rotation_.y = 3.1f;

	Vector3 velocity = { 1.0f,1.0f,0.0f };
	particle.reset(ParticleSystem::Create(circle, camera, velocity, true));
	particle->emitter_->count = 100;
	particle->emitter_->transform.scale = { 0.5f,0.5f,0.0f };
	particle->SetLifeTime(1.0f, 3.0f);
	particle->SetRandomAllVelocity();
	particle->SetRandomColor();

	//particle->SetisInvisible(true);

	Vector3 velocity2 = { 0.0f,5.0f,5.0f };
	particle2.reset(ParticleSystem::Create(uvTexture, camera, velocity2, true));
	particle2->emitter_->frequency = 0.3f;
	particle2->emitter_->count = 10;
	particle2->emitter_->transform.scale = { 1.0f,1.0f,1.0f };
	particle2->SetLifeTime(1.0f, 3.0f);
	particle2->SetRandomAllVelocity();
	//particle2->SetisInvisible(true);
}

void TitleScene::Update()
{
	camera->CameraDebug();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_UP))
	{
		camera->transform.translate.z += 0.03f;
	}
	if (input->PushKey(DIK_S) || input->PushKey(DIK_DOWN))
	{
		camera->transform.translate.z -= 0.03f;
	}
	if (input->PushKey(DIK_A) || input->PushKey(DIK_LEFT))
	{
		camera->transform.translate.x -= 0.03f;
	}
	if (input->PushKey(DIK_D) || input->PushKey(DIK_RIGHT))
	{
		camera->transform.translate.x += 0.03f;
	}

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
			Audio::GetInstance()->SoundStopWave(soundData);
		}
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
		Audio::GetInstance()->SoundStopWave(soundData);
	}

	fence_->Update();
	cube_->Update();



	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());


}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

}

