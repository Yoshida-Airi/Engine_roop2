#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete uiCamera;
	delete particle;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	fence_.reset(Model::Create("Resources", "fence.obj"));
	cube_.reset(Model::Create("Resources", "fence.obj"));

	circle = TextureManager::GetInstance()->LoadTexture("Resources/circle.png");

	
	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.translate = { 10.8f,0.2f,0.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	particle = Particle::Create(circle, emitter);


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

	particle->Update();




	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
	fence_->Parent(cube_.get());

}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

	particle->Draw(camera);

}

