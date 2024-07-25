#include "TestPlayScene.h"

#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera;
	delete levelEditor;
}

void TestPlayScene::Initialize()
{
	camera = new Camera;
	camera->Initialize();

	walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/walk.gltf");

	model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	model->SetisInvisible(true);
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	model2->GetWorldTransform()->translation_.x = 5.0f;
	model2->SetisInvisible(true);

	model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	model3->SetisInvisible(true);
	//model3->SetAnimation(walkAnimation);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4->GetWorldTransform()->rotation_.y = 3.14f;
	model4->SetisInvisible(true);

	levelEditor = new LevelEditor();
	levelEditor->LoaderJsonFile();
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

	levelEditor->Update();
}

void TestPlayScene::Draw()
{
	levelEditor->Draw(camera);

	model->Draw(camera);
	model2->Draw(camera);
	model3->Draw(camera);
	model4->Draw(camera);
}

