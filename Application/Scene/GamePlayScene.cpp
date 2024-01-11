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

	object = ModelLoader::GetInstance();
	plane = object->LoadObjFile("Resources", "plane.obj");
	cube = object->LoadObjFile("Resources", "cube.obj");

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	triangle = triangle->Create(uvTexture);
	triangle2 = triangle2->Create(monsterBall);
	
	//triangle->SetisInvisible(true);
	//triangle2->SetisInvisible(true);


	sprite = sprite->Create(Doll);
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });
	
	//sprite->SetisInvisible(true);


	sprite2 = sprite->Create(uvTexture);
	sprite2->SetSize({ 64.0f, 64.0f });
	sprite2->SetTextureLeftTop({ 0,0 });
	/*sprite2->SetisInvisible(true);*/

	sphere = sphere->Create(monsterBall);
	sphere->SetisInvisible(true);

	model = model->Create(plane);
	model2 = model2->Create(cube);

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


	model->Update();
	model2->Update();
	model->worldTransform_.translation_.x = 3.0f;


}

void GamePlayScene::Draw()
{

	triangle->Draw(camera);
	triangle2->Draw(camera);

	

	sphere->Draw(camera);


	model->Draw(camera);
	model2->Draw(camera);


	sprite->Draw(uiCamera);
	sprite2->Draw(uiCamera);

}
