#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete uiCamera;
	//delete efect;
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

	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
	Enter = texture->LoadTexture("Resources/Enter.png");
	circle = texture->LoadTexture("Resources/circle.png");

	sprite.reset(Sprite::Create(Enter));
	//sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });


	sprite2.reset(Sprite::Create(uvTexture));
	sprite2->SetSize({ 64.0f, 64.0f });
	sprite2->SetTextureLeftTop({ 0,0 });

	fence_.reset(Model::Create("Resources", "fence.obj"));
	cube_.reset(Model::Create("Resources", "cube.obj"));

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;
	emitter.transform.translate = { 0.0f,0.2f,0.0f };
	emitter.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter.transform.scale = { 1.0f,1.0f,1.0f };
	particle.reset(ParticleSystem::Create(circle, emitter));


}

void TitleScene::Update()
{
#ifdef _DEBUG
	camera->CameraDebug();

	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");
#endif // _DEBUG



	input->TriggerKey(DIK_0);
	

	if (input->TriggerKey(DIK_RETURN))
	{
		
		sceneManager_->ChangeScene("GAMEPLAY");
	}


#ifdef _DEBUG
	ImGui::Begin("scene");

	ImGui::Text("Required assignment");


	if (ImGui::Button("+a assignment"))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	ImGui::End();
#endif // _DEBUG




	fence_->Update();
	cube_->Update();

	fence_->Parent(cube_.get());
	sprite->worldTransform_->translation_ = { 700.0f };

	sprite->Update();
	sprite2->Update();

	particle->Update();

	
}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);

	particle->Draw(camera);

	sprite->Draw(uiCamera);
	sprite2->Draw(uiCamera);

}

