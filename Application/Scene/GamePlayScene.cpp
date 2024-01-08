#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{
	//delete triangle;
	//delete triangle2;
	//delete sprite;
	//delete sprite2;
	//delete sphere;
	//delete model;
	//delete model2;

	delete player;
	delete enemy;
	delete skydome;

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


	object = ModelLoader::GetInstance();
	playerData = object->LoadObjFile("Resources/Player", "player.obj");
	playerBulletData = object->LoadObjFile("Resources/Player", "playerBullet.obj");
	EnemyData= object->LoadObjFile("Resources/Enemy", "enemy.obj");
	EnemyBulletData = object->LoadObjFile("Resources/Enemy", "enemyBullet.obj");
	skydomeData= object->LoadObjFile("Resources/Skydome", "skydome.obj");

//	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
//	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
//	Doll = texture->LoadTexture("Resources/Doll.png");
//
//	object = ModelLoader::GetInstance();
//	plane = object->LoadObjFile("Resources", "plane.obj");
//	cube = object->LoadObjFile("Resources", "cube.obj");
//
	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	
	skydome = new Skydome();
	skydome->Initialize(camera, skydomeData, { 0,0,0 });

	player = new Player();
	player->Initialize(camera,uiCamera, playerData,playerBulletData);

	// 敵の速度
	const float kEnemySpeed = -0.0f;
	const float kEnemySpeedB = 0.2f;
	Vector3 velocityA(0, 0, kEnemySpeed);
	Vector3 velocityB(kEnemySpeed, kEnemySpeedB, 0);
	enemy = new Enemy();
	enemy->Initialize(camera, EnemyData,EnemyBulletData, { 0.0f,2.0f,30.0f }, velocityA, velocityB);
	enemy->SetPlayer(player);
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

	//当たり判定
	CheackAllColoisions();


}

void GamePlayScene::Draw()
{
	skydome->Draw();
	player->Draw();
	enemy->Draw();

	player->DrawUI();

}

void GamePlayScene::CheackAllColoisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy->GetBullets();

	//半径
	const float radius = 2.0f;

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };

		//球同士が当たっていれば
		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <= (radius * radius))
		{
			//自キャラの衝突時コールバックを呼び出す
			player->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = enemy->GetWorldPosition();

	//自弾と敵キャラの全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離を求める
		Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };

		// 球同士が当たっていれば
		if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <= (radius * radius)) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets)
	{
		for (EnemyBullet* enemyBullet : enemyBullets)
		{
			//自弾の座標
			posA = playerBullet->GetWorldPosition();
			//敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };


			// 球同士が当たっていれば
			if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
				(radius * radius)) {
				// 敵キャラの衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}

		}
	}

#pragma endregion
}