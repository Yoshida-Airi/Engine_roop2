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
	
	//triangle->SetisInvisible(true);
	//triangle2->SetisInvisible(true);

	//
	/*sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });*/
	
	//sprite->SetisInvisible(true);

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.translate = { 0.0f,0.2f,0.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	sprite2.reset(Sprite::Create(circle,emitter));
	/*sprite2->SetSize({ 64.0f, 64.0f });
	sprite2->SetTextureLeftTop({ 0,0 });*/
	/*sprite2->SetisInvisible(true);*/

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.translate = { 10.8f,0.2f,0.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	sphere.reset(Sphere::Create(monsterBall));
	//sphere->SetisInvisible(true);

	model.reset(Model::Create("Resources", "plane.obj"));
	model2.reset(Model::Create("Resources", "plane.obj"));

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
	triangle->worldTransform_->rotation_.y += 0.03f;

	triangle2->Update();
	triangle2->worldTransform_->scale_.y = 0.5f;
	triangle2->worldTransform_->rotation_.y += 0.02f;

	//sprite->worldTransform_->translation_ = { 700.0f };

	//sprite->Update();
	sprite2->Update();

	//sprite->Debug("Doll");
	sprite2->Debug("uv");

	sphere->Update();
	sphere->worldTransform_->rotation_.y += 0.01f;


	model->Update();
	model2->Update();
	model->worldTransform_->translation_.x = 3.0f;


}

void GamePlayScene::Draw()
{

	triangle->Draw(camera);
	triangle2->Draw(camera);

	

	sphere->Draw(camera);


	model->Draw(camera);
	model2->Draw(camera);


	/*sprite->Draw(uiCamera);*/
	sprite2->Draw(camera);

}
