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

	walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/walk.gltf");

	model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	model2->GetWorldTransform()->translation_.x = 5.0f;

	model3.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	//model3->SetAnimation(walkAnimation);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));

}

void TestPlayScene::Update()
{
	camera->CameraDebug();
	model->Update();
	model2->Update();
	model3->Update();
	model4->Update();

	model->ModelDebug("model");
	model4->ModelDebug("simpleSkin");

	
}

void TestPlayScene::Draw()
{
	model->Draw(camera);
	model2->Draw(camera);
	model3->Draw(camera);
	model4->Draw(camera);
}

