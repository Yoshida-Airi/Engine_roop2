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


	model.reset(Model::Create("Resources/SampleAssets/AnimatedCube/AnimatedCube.gltf"));
	Animation animation = ModelLoader::GetInstance()->LoadAnimationFile("Resources/SampleAssets/AnimatedCube"," AnimatedCube.gltf");
}

void TestPlayScene::Update()
{
	camera->CameraDebug();
	model->Update();

	model->ModelDebug("model");

}

void TestPlayScene::Draw()
{
	model->Draw(camera);
}
