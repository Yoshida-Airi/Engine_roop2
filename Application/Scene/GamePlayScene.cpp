#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;

}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG


	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
	Doll = texture->LoadTexture("Resources/Doll.png");
	circle = texture->LoadTexture("Resources/circle.png");

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	triangle.reset(Triangle::Create(uvTexture));
	triangle2.reset(Triangle::Create(monsterBall));
	


	sphere.reset(Sphere::Create(monsterBall));
	sphere->worldTransform_->translation_.x = 2.0f;

}

void GamePlayScene::Update()
{
	input->TriggerKey(DIK_0);

#ifdef _DEBUG
	
	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

#ifdef _DEBUG
	ImGui::Begin("scene");

	ImGui::Text("+a assignment");


	if (ImGui::Button("Required assignment"))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	ImGui::End();
#endif // #ifdef _DEBUG

	

	triangle->Update();
	triangle->worldTransform_->rotation_.y += 0.03f;

	triangle2->Update();
	triangle2->worldTransform_->scale_.y = 0.5f;
	triangle2->worldTransform_->rotation_.y += 0.02f;


	sphere->Update();
	sphere->worldTransform_->rotation_.y += 0.01f;

}

void GamePlayScene::Draw()
{

	triangle->Draw(camera);
	triangle2->Draw(camera);

	

	sphere->Draw(camera);


}
