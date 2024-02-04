#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete uiCamera;
	delete efect;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	texture = TextureManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	monsterBall = texture->LoadTexture("Resources/monsterBall.png");

	fence_.reset(Model::Create("Resources", "fence.obj"));
	cube_.reset(Model::Create("Resources", "fence.obj"));

	sphere.reset(Sphere::Create(monsterBall));

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

	sphere->Update();
	sphere->worldTransform_->rotation_.y += 0.03f;

}

void TitleScene::Draw()
{
	/*fence_->Draw(camera);
	cube_->Draw(camera);*/
	sphere->Draw(camera);
}

