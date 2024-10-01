#include "TestPlayScene.h"

#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera;
	delete levelEditor;
	//delete pointLight;
}

void TestPlayScene::Initialize()
{
	camera = new Camera;
	camera->Initialize();

	walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/walk.gltf");

	pointLight = new PointLight();
	pointLight->position = { 0.0,2.0f,0.0f };
	pointLight->intensity = { 1.0f };
	pointLight->color = { 1.0f,0.0f,0.0f,1.0f };



	//model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/terrain.obj"));
	model2->GetWorldTransform()->translation_.x = 5.0f;


	//model2->SetisInvisible(true);

	//model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	//model3->SetAnimation(walkAnimation);

	//model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	//model4->GetWorldTransform()->rotation_.y = 3.14f;

	/*levelEditor = new LevelEditor();
	levelEditor->LoaderJsonFile();*/

	model2->SetLight(pointLight);
	
}

void TestPlayScene::Update()
{
	ImGui::Begin("spotLight");
	ImGui::SliderFloat3("spotlight", &pointLight->position.x, -10.0f, 10.0f);
	ImGui::End();

	camera->CameraDebug();
	//model->Update();
	model2->Update();
	//model3->Update();
	//model4->Update();


	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		model->MoveAnimation(false);
	}

	//model->ModelDebug("model");
	model2->ModelDebug("cube");
	//model3->ModelDebug("walk");
	//model4->ModelDebug("simpleSkin");

	//levelEditor->Update();
}

void TestPlayScene::Draw()
{
	
	model2->Draw(camera);
	//model->Draw(camera);

	//model3->Draw(camera);
	//model4->Draw(camera);

	//levelEditor->Draw(camera);
}

