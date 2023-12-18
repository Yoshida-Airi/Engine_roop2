#include "Title.h"

Title::~Title()
{

	delete triangle;
	delete triangle2;
	delete sprite;
	delete sprite2;
	delete sphere;
	delete model;
	delete model2;

	delete camera;
	delete uiCamera;


	delete input;
	input = nullptr;


#ifdef _DEBUG
	delete imgui;
#endif // _DEBUG



}

void Title::Initialize()
{
	texture = TextureManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
	Doll = texture->LoadTexture("Resources/Doll.png");

	object = ModelLoader::GetInstance();
	plane = object->LoadObjFile("Resources", "plane.obj");
	cube = object->LoadObjFile("Resources", "cube.obj");

	uiCamera = new UICamera;
	uiCamera->Initialize();

	input = Input::GetInstance();
	input->Initialize();


#ifdef _DEBUG

	imgui = ImGuiManager::GetInstance();
	imgui->Initialize();
#endif // _DEBUG



	triangle = new Triangle;
	triangle->Initialize(uvTexture);
	triangle2 = new Triangle;
	triangle2->Initialize(monsterBall);
	triangle->SetisInvisible(true);
	triangle2->SetisInvisible(true);


	sprite = new Sprite;
	sprite->Initialize(Doll);
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 64,0 });
	sprite->SetTextureSize({ 64.0f, 64.0f });
	sprite->SetisInvisible(true);


	sprite2 = new Sprite;
	sprite2->Initialize(uvTexture);
	sprite2->SetPosition({ 100,100 });
	sprite2->SetSize({ 640.0f, 360.0f });
	sprite2->SetisInvisible(true);

	sphere = new Sphere;
	sphere->Initialize(monsterBall);
	sphere->SetisInvisible(true);

	model = new Model;
	model->Initialize(plane);
	model->SetisInvisible(true);

	model2 = new Model;
	model2->Initialize(cube);
	model2->SetisInvisible(true);

}

void Title::Update()
{

#ifdef _DEBUG
	imgui->Begin();
	camera->CameraDebug();
	sprite2->Debug();

#endif // _DEBUG

	model->Update();
	model2->Update();
	model->worldTransform_.translation_.x = 3.0f;

	input->Update();
	input->TriggerKey(DIK_0);

	triangle->Update();
	triangle->worldTransform.rotation_.y += 0.03f;

	triangle2->Update();
	triangle2->worldTransform.scale_.y = 0.5f;
	triangle2->worldTransform.rotation_.y += 0.02f;

	sprite->worldTransform.translation_ = { 700.0f };


	sprite->Update();
	sprite2->Update();

	sphere->Update();
	sphere->worldTransform_.rotation_.y += 0.01f;

}

void Title::Draw()
{
	triangle->Draw(camera);
	triangle2->Draw(camera);

	sprite->Draw(uiCamera);
	sprite2->Draw(uiCamera);

	sphere->Draw(camera);

	model->Draw(camera);
	model2->Draw(camera);

#ifdef _DEBUG
	ImGui::ShowDemoWindow();
	imgui->End();
	imgui->Draw();
#endif // _DEBUG

}
