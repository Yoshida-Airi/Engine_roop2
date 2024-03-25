#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{
	delete camera;
}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	//画像読み込み
	uvTexture = texture->LoadTexture("Resources/DefaultAssets/uvChecker.png");
	Doll = texture->LoadTexture("Resources/DefaultAssets/Doll.png");
	
	//スプライト生成
	sprite.reset(Sprite::Create(Doll));
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 0,0 });

	sprite2.reset(Sprite::Create(uvTexture));

}

void GamePlayScene::Update()
{
	//シーン遷移
	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	sprite->GetWorldTransform()->translation_ = { 700.0f };

	sprite->Update();
	sprite2->Update();

	//デバッグ
	sprite->Debug("Doll");
	sprite2->Debug("uv");
	camera->CameraDebug();

}

void GamePlayScene::Draw()
{
	sprite->Draw(camera);
	sprite2->Draw(camera);
}
