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

	uint32_t soundData = Audio::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	Audio::GetInstance()->SoundPlayWave(soundData, false);

	camera = new Camera;
	camera->Initialize();



	fence_.reset(Model::Create("DefaultAssets/fence.obj"));
	cube_.reset(Model::Create("DefaultAssets/cube.obj"));

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
	fence_->Parent(cube_.get());


}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

}

