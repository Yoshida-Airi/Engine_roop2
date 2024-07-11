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
	uint32_t white = textureManager_->LoadTexture("Resources/SampleAssets/white.png");

	skybox.reset(Skybox::Create(airport));
	skybox->GetWorldTransform()->scale_ = { 10.0f,10.0f,10.0f };

	//walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/sneakWalk.gltf");

	model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	model->SetEnvironmentMap(airport);
	model->SetisInvisible(true);
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/axis.obj"));
	model2->SetTexture(white);
	model2->SetEnvironmentMap(airport);
	model2->GetWorldTransform()->translation_.x = 5.0f;
	model2->SetMaterialData({ 0.0f,0.0f,0.0f,1.0f });

	model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	model3->SetEnvironmentMap(airport);
	model3->SetMaterialData({ 0.0f,0.0f,0.0f,1.0f });
	
	//model3->SetAnimation(walkAnimation);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4->GetWorldTransform()->rotation_.y = 3.14f;
	model4->SetisInvisible(true);
	model4->SetEnvironmentMap(airport);
	

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

	model2->GetWorldTransform()->rotation_.y += 0.01f;
	model3->GetWorldTransform()->rotation_.y += 0.01f;

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

