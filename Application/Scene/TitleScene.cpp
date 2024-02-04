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

	sphere.reset(Sphere::Create(monsterBall));

}

void TitleScene::Update()
{
	camera->CameraDebug();

	sphere->Update();
	sphere->worldTransform_->rotation_.y += 0.03f;

}

void TitleScene::Draw()
{

	sphere->Draw(camera);
}

