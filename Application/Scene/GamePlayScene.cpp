#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;

	delete skydome;
	delete player;
	delete enemy;

}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

#ifdef _DEBUG
	imgui = ImGuiManager::GetInstance();
#endif // _DEBUG

	//当たり判定処理の設定
	colliderManager_ = new CollisionManager;

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	skydome = new Skydome();
	skydome->Initialize();

	player = new Player();
	player->Initialize({-5.0f,0.0f,0.0f});

	enemy = new Enemy();
	enemy->SetPlayer(player);
	enemy->Initialize({10.0f,0.2f,30.0f});

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

	skydome->Update();
	player->Update();
	enemy->Update();
	

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy->GetBullets();


	//リストをクリアする
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録する
	colliderManager_->AddColliders(player);
	colliderManager_->AddColliders(enemy);
	for (PlayerBullet* playerBullet : playerBullets) {
		colliderManager_->AddColliders(playerBullet);
	}
	for (EnemyBullet* enemyBullet : enemyBullets) {
		colliderManager_->AddColliders(enemyBullet);
	}

	//当たり判定
	colliderManager_->ChackAllCollisions();

}

void GamePlayScene::Draw()
{

	skydome->Draw(camera);
	player->Draw(camera);
	enemy->Draw(camera);

}
