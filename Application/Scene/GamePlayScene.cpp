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

	// 敵のデータ取得
	LoadEnemyPopData();

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
	player->Initialize({ 0.0f,0.0f,30.0f }, camera);
	

	player->SetParent(&railCamera->GetWorldTransform());
	
	fadeTex = TextureManager::GetInstance()->LoadTexture("Resources/black.png");

	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(false);
	alpha = 1.0f;
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,alpha });

	StartFadeOut();
}

void GamePlayScene::Update()
{


	if (isFadingOut == true)
	{
		UpdateFadeOut();
	}


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

		//敵の生成
		UpdateEnemyPopCommands();

		// 敵キャラの更新
		for (Enemy* enemy : enemys) {
			enemy->Update();
		}

		//弾の更新
		for (EnemyBullet* bullet : enemyBullets_)
		{
			bullet->Update();
		}

		fadeSprite->Update();
	
	
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
	player->DrawUI(uiCamera);

	fadeSprite->Draw(uiCamera);

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

void GamePlayScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Csv/EnemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (isWaitTime_)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			//待機完了
			isWaitTime_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 一桁分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先端文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			SpawnEnemy(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWaitTime_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}


void GamePlayScene::StartFadeOut()
{
	isFadingOut = true;
	fadeSprite->SetisInvisible(false);
}

void GamePlayScene::UpdateFadeOut()
{
	alpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, alpha });

	if (alpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadingOut = false;
	}
}


