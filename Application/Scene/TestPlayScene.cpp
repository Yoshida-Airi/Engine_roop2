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


	model.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	//model->UseAnimation(false);
	//model2.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	//model2->GetWorldTransform()->translation_.x = 5.0f;
}

void TestPlayScene::Update()
{
	camera->CameraDebug();
	model->Update();
	//model2->Update();

	model->ModelDebug("model");

	
}

void TestPlayScene::Draw()
{
	model->Draw(camera);
	//model2->Draw(camera);
}

