#include "TitleScene.h"

TitleScene::~TitleScene()
{
	delete camera;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	texture = TextureManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	//画像読み込み
	uvTexture = texture->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	circle = texture->LoadTexture("Resources/DefaultAssets/circle.png");

	//モデル生成
	fence_.reset(Model::Create("Resources/DefaultAssets/fence.obj"));
	cube_.reset(Model::Create("Resources/DefaultAssets/cube.obj"));
	fence_->GetWorldTransform()->rotation_.y = 3.1f;

	//パーティクル生成
	Vector3 velocity = { 1.0f,1.0f,0.0f };
	particle.reset(ParticleSystem::Create(circle, camera, velocity, true));
	particle->emitter_->count = 100;
	particle->emitter_->transform.scale = { 0.5f,0.5f,0.0f };
	particle->emitter_->transform.translate.x = 7.0f;
	particle->SetLifeTime(1.0f, 3.0f);
	particle->SetRandomAllVelocity();
	particle->SetRandomColor();

	Vector3 velocity2 = { 0.0f,5.0f,5.0f };
	particle2.reset(ParticleSystem::Create(uvTexture, camera, velocity2, true));
	particle2->emitter_->frequency = 0.3f;
	particle2->emitter_->count = 10;
	particle2->emitter_->transform.scale = { 1.0f,1.0f,1.0f };
	particle2->emitter_->transform.translate.x = -7.0f;
	particle2->SetLifeTime(1.0f, 3.0f);
	particle2->SetRandomAllVelocity();
}

void TitleScene::Update()
{
	
	//カメラ移動
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

	camera->UpdateMatrix();

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	//シーン遷移
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
		}
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	fence_->Update();
	cube_->Update();

	particle->Update();
	particle2->Update();


	//デバッグ
	camera->CameraDebug();
	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

	particle->Debug("circleParticle");
	particle2->Debug("uvTextureParticle");


}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);
	particle->Draw();
	particle2->Draw();
}

