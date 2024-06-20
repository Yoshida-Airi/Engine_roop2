#include "TestPlayScene.h"

#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera;
}

void TestPlayScene::Initialize()
{
	camera = new Camera;
	camera->Initialize();

	textureManager_ = TextureManager::GetInstance();

	uint32_t airport = textureManager_->LoadTexture("Resources/SampleAssets/rostock_laage_airport_4k.dds");

	skybox.reset(Skybox::Create(airport));
	skybox->GetWorldTransform()->scale_ = { 3.0f,3.0f,3.0f };

	//walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/sneakWalk.gltf");

	model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	model2->GetWorldTransform()->translation_.x = 5.0f;

	model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	//model3->SetAnimation(walkAnimation);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4->GetWorldTransform()->rotation_.y = 3.14f;

	

}

void TestPlayScene::Update()
{
	camera->CameraDebug();
	model->Update();
	model2->Update();
	model3->Update();
	model4->Update();

	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		model->MoveAnimation(false);
	}

	model->ModelDebug("model");
	model2->ModelDebug("cube");
	model3->ModelDebug("walk");
	model4->ModelDebug("simpleSkin");

	skybox->Update();
	
}

void TestPlayScene::Draw()
{
	model->Draw(camera);
	model2->Draw(camera);
	model3->Draw(camera);
	model4->Draw(camera);

	skybox->Draw(camera);
}

