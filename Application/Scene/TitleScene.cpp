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

	camera = new Camera;
	camera->Initialize();

	fence_.reset(Model::Create("Resources", "fence.obj"));
	cube_.reset(Model::Create("Resources", "fence.obj"));

	uvTexture = TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	particle.reset(Particle::Create(uvTexture));


}

void TitleScene::Update()
{
	camera->CameraDebug();

	input->TriggerKey(DIK_0);

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	fence_->Update();
	cube_->Update();

	particle->Update();


	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

	particle->Draw(camera);

}

