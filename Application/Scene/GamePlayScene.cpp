#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;
	delete railCamera;

	delete skydome;
	delete player;
	delete enemy;

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

	enemy = new Enemy();
	enemy->SetPlayer(player);
	enemy->SetGameScene(this);
	enemy->Initialize({10.0f,0.2f,30.0f});
	

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
	colliderManager_->AddColliders(enemy);
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
	enemy->Update();

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
	enemy->Draw(camera);

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
