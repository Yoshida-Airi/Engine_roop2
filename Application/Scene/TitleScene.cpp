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


	fence_ = Model::Create("Resources", "fence.obj");
	cube_ = Model::Create("Resources", "cube.obj");

	
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


	
	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");

	
	
}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);
}

