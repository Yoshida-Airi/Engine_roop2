#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;
	delete railCamera;

	delete skydome;
	delete player;
	for (Enemy* enemy : enemys)
	{
		delete enemy;
	}

	for (EnemyBullet* bullet : enemyBullets_)
	{
		delete bullet;
	}

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
	player->Initialize({0.0f,0.0f,30.0f});
	
	SpawnEnemy({ 0.0f,0.2f,30.0f });
	SpawnEnemy({ 1.0f,0.2f,30.0f });
	

	player->SetParent(&railCamera->GetWorldTransform());

}

void GamePlayScene::Update()
{

	//デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});

	// デスフラグの立った敵を削除
	enemys.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
		});

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
	//// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy->GetBullets();

	//リストをクリアする
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録する
	colliderManager_->AddColliders(player);
	for (Enemy* enemy : enemys) {
		colliderManager_->AddColliders(enemy);
	}
	for (PlayerBullet* playerBullet : playerBullets) {
		colliderManager_->AddColliders(playerBullet);
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		colliderManager_->AddColliders(enemyBullet);
	}

	//当たり判定
	colliderManager_->ChackAllCollisions();

	skydome->Update();
	player->Update();

	// 敵キャラの更新
	for (Enemy* enemy : enemys) {
		enemy->Update();
	}

	//弾の更新
	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Update();
	}
	
}

void GamePlayScene::Draw()
{

	skydome->Draw(camera);
	player->Draw(camera);
	// 敵キャラ
	for (Enemy* enemy : enemys) {
		enemy->Draw(camera);
	}

	//弾の描画
	for (EnemyBullet* bullet : enemyBullets_)
	{
		bullet->Draw(camera);
	}

}

void GamePlayScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GamePlayScene::SpawnEnemy(const Vector3& position)
{
	// 敵を発生させる
	Enemy* enemy = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy->SetPlayer(player);
	enemy->SetGameScene(this);
	// 敵の初期化
	enemy->Initialize(position);

	// リストに登録
	enemys.push_back(enemy);
}
