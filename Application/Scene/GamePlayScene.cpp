#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;
	delete railCamera;

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

	railCameraWorldTransform_.Initialize();

	railCamera = new RailCamera();
	railCamera->Initialize(railCameraWorldTransform_, railCameraWorldTransform_.rotation_);

	skydome = new Skydome();
	skydome->Initialize();

	player = new Player();
	player->Initialize({-5.0f,0.0f,0.0f});

	enemy = new Enemy();
	enemy->SetPlayer(player);
	enemy->Initialize({10.0f,0.2f,30.0f});

	player->SetParent(&railCamera->GetWorldTransform());

}

void GamePlayScene::Update()
{

	camera->UpdateMatrix();

#ifdef _DEBUG
	
	camera->CameraDebug();

#endif // _DEBUG

	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	//レールカメラの更新
	railCameraWorldTransform_.UpdateWorldMatrix();
	railCamera->Update();
	camera->matView = railCamera->GetCamera()->matView;
	camera->matProjection = railCamera->GetCamera()->matProjection;

	camera->TransferMatrix();



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

	skydome->Update();
	player->Update();
	enemy->Update();
	
}

void GamePlayScene::Draw()
{

	skydome->Draw(camera);
	player->Draw(camera);
	enemy->Draw(camera);

}
