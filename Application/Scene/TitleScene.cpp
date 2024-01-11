#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete fence_;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera;
	camera->Initialize();


	object = ModelLoader::GetInstance();
	fenceData = object->LoadObjFile("Resources", "fence.obj");

	fence_ = fence_->Create(fenceData);
	
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
}

void TitleScene::Draw()
{
	fence_->Draw(camera);
}

