#include "TestPlayScene.h"

#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera;
	delete jsonLoder;
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

	model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	//model3->SetAnimation(walkAnimation);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4->GetWorldTransform()->rotation_.y = 3.14f;

	jsonLoder = new JsonLoader();
	jsonLoder->LoaderJsonFile();
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

	jsonLoder->Update();
}

void TestPlayScene::Draw()
{
	jsonLoder->Draw(camera);

	model->Draw(camera);
	model2->Draw(camera);
	model3->Draw(camera);
	model4->Draw(camera);
}

