#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete uiCamera;

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


	uvTexture = texture->LoadTexture("Resources/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/monsterBall.png");
	Doll = texture->LoadTexture("Resources/Doll.png");


	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();


	//triangle = triangle->Create(uvTexture);
	//triangle2 = triangle2->Create(monsterBall);
	//
	////triangle->SetisInvisible(true);
	////triangle2->SetisInvisible(true);


	//sprite = sprite->Create(Doll);
	//sprite->SetSize({ 64.0f, 64.0f });
	//sprite->SetTextureLeftTop({ 0,0 });
	//
	////sprite->SetisInvisible(true);


	//sprite2 = sprite->Create(uvTexture);
	//sprite2->SetSize({ 64.0f, 64.0f });
	//sprite2->SetTextureLeftTop({ 0,0 });
	///*sprite2->SetisInvisible(true);*/

	//sphere = sphere->Create(monsterBall);
	//sphere->SetisInvisible(true);


	enemyModel = Model::Create("Resources", "cube.obj");
	enemyBulletModel = Model::Create("Resources", "plane.obj");

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

	//triangle->Update();
	//triangle->worldTransform.rotation_.y += 0.03f;

	//triangle2->Update();
	//triangle2->worldTransform.scale_.y = 0.5f;
	//triangle2->worldTransform.rotation_.y += 0.02f;

	//sprite->worldTransform.translation_ = { 700.0f };

	//sprite->Update();
	//sprite2->Update();

	//sphere->Update();
	//sphere->worldTransform_.rotation_.y += 0.01f;


	//model->Update();
	//model2->Update();
	//model->worldTransform_.translation_.x = 3.0f;

	player->Update();
	enemy->Update();

	CheackAllCollisions();

}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//triangle2->Draw(camera);

	//

	//sphere->Draw(camera);


	//model->Draw(camera);
	//model2->Draw(camera);


	//sprite->Draw(uiCamera);
	//sprite2->Draw(uiCamera);

	player->Draw(camera);
	enemy->Draw(camera);

}

void GamePlayScene::CheackAllCollisions()
{
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy->GetBullets();


	//コライダー
	std::list<Collider*> colliders_;

	//コライダーをリストに登録
	colliders_.push_back(player);
	colliders_.push_back(enemy);

	//自弾全てについて
	for (PlayerBullet* playerBullet : playerBullets)
	{
		colliders_.push_back(playerBullet);
	}

	//敵弾全てについて
	for (EnemyBullet* enemyBullet : enemyBullets)
	{
		colliders_.push_back(enemyBullet);
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA)
	{
		//イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)

		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB)
		{
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			//コライダーAトコライダーBの当たり判定
			CheackCollisionPair(colliderA, colliderB);

		}
	}

}

void GamePlayScene::CheackCollisionPair(Collider* colliderA, Collider* colliderB)
{
	//衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != colliderB->GetCollisionMask() ||
		colliderB->GetCollisionAttribute() != colliderA->GetCollisionMask())
	{
		return;
	}

	// コライダーAのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	// コライダーBのワールド座標を取得
	Vector3 posB = colliderB->GetWorldPosition();

	// 座標AとBの距離を求める
	Vector3 distance = { posB.x - posA.x, posB.y - posA.y, posB.z - posA.z };

	// 球と球の交差判定
	if ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z) <=
		(colliderA->GetRadius() * colliderB->GetRadius())) {
		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}
