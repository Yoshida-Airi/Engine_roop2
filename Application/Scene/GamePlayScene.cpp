#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete levelEditor;
	delete cameraController;

	for (Enemy* enemy : enemys)
	{
		delete enemy;
	}

}

void GamePlayScene::Initialize()
{
	texture = TextureManager::GetInstance();
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	//当たり判定処理の設定
	colliderManager_ = std::make_unique<CollisionManager>();
	colliderManager_->Initialize();


	//テクスチャの読み込み
	uvTexture = texture->LoadTexture("Resources/SampleAssets/uvChecker.png");
	monsterBall = texture->LoadTexture("Resources/SampleAssets/monsterBall.png");
	Doll = texture->LoadTexture("Resources/SampleAssets/Doll.png");
	circle = texture->LoadTexture("Resources/SampleAssets/circle.png");


	camera = new Camera;
	camera->Initialize();

	
	levelEditor = new LevelEditor();
	levelEditor->LoaderJsonFile();
	levelEditor->Initialize();

	weapon = std::make_unique<Weapon>();
	weapon->Initialize();


	ground = std::make_unique<Ground>();
	ground->Initialize();

	player = std::make_unique<Player>();
	player->SetWeapon(weapon.get());
	player->SetGround(ground.get());
	player->Initialize();
	

	SpawnEnemy({ 10.0f,5.0f,0.0f });
	SpawnEnemy({ 10.0f,1.0f,0.0f });

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();

	cameraController = new CameraController;
	cameraController->Initialize(camera);
	cameraController->SetTarget(player.get());
	cameraController->Reset();


	//triangle.reset(Triangle::Create(uvTexture));
	//triangle2.reset(Triangle::Create(monsterBall));
	//
	//triangle->SetisInvisible(true);
	//triangle2->SetisInvisible(true);

	////
	//sprite.reset(Sprite::Create(Doll));
	//sprite->SetSize({ 64.0f, 64.0f });
	//sprite->SetTextureLeftTop({ 0,0 });
	//
	////sprite->SetisInvisible(true);


	//sprite2.reset(Sprite::Create(uvTexture));
	//sprite2->SetSize({ 64.0f, 64.0f });
	//sprite2->SetTextureLeftTop({ 0,0 });
	////sprite2->SetisInvisible(true);

	//sphere.reset(Sphere::Create(monsterBall));
	//sphere->GetWorldTransform()->translation_.y = -1.0f;
	////sphere->SetisInvisible(true);

	//model.reset(Model::Create("Resources/SampleAssets/plane.gltf"));
	//model2.reset(Model::Create("Resources/SampleAssets/terrain.obj"));

	//model->GetWorldTransform()->rotation_.y = 3.14f;
	//model2->GetWorldTransform()->rotation_.y = 3.14f;

	//model2->GetWorldTransform()->translation_ =
	//{
	//	0.0f,-1.5,0.0f
	//};

	////model->SetisInvisible(true);
	////model2->SetisInvisible(true);

	//Vector3 velocity = { 1.0f,1.0f,0.0f };
	//particle.reset(ParticleSystem::Create(circle, camera, velocity, true,false));
	//particle->emitter_->count = 100;
	//particle->emitter_->transform.scale = { 0.5f,0.0f,0.0f };
	//particle->SetLifeTime(1.0f, 3.0f);
	////particle->SetisInvisible(true);

	//Vector3 velocity2 = { 0.0f,5.0f,5.0f };
	//particle2.reset(ParticleSystem::Create(uvTexture, camera, velocity2, true, true));
	//particle2->emitter_->frequency = 0.1f;
	//particle2->SetLifeTime(0.1f, 0.5f);
	////particle2->SetisInvisible(true);


}

void GamePlayScene::Update()
{
	input->TriggerKey(DIK_0);

#ifdef _DEBUG

	camera->CameraDebug();

#endif // _DEBUG

	cameraController->Update();

	colliderManager_->UpdateWorldTransform();

	//if (input->TriggerKey(DIK_RETURN))
	//{
	//	sceneManager_->ChangeScene("TITLE");
	//}

	//triangle->Update();
	//triangle->GetWorldTransform()->rotation_.y += 0.03f;

	//triangle2->Update();
	//triangle2->GetWorldTransform()->scale_.y = 0.5f;
	//triangle2->GetWorldTransform()->rotation_.y += 0.02f;

	//sprite->GetWorldTransform()->translation_ = { 700.0f };

	//sprite->Update();
	//sprite2->Update();

	//sprite->Debug("Doll");
	//sprite2->Debug("uv");

	//sphere->Update();
	//sphere->GetWorldTransform()->rotation_.y += 0.01f;

	//model->ModelDebug("plane");
	//model2->ModelDebug("plane2");

	//model->Update();
	//model2->Update();
	//model->GetWorldTransform()->translation_.x = 3.0f;

	//particle->Debug("circleParticle");
	//particle2->Debug("uvTextureParticle");

	//particle->Update();
	//particle2->Update();



	levelEditor->Update();
	ground->Update();

	player->Update();
	//武器の更新
	weapon->Update();

	for (Enemy* enemy : enemys) 
	{
		enemy->Update();
	}

	skydome->Update();

	CheckAllCollisions();

	//camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);
	//camera->transform.translate.y = LerpShortTranslate(camera->transform.translate.y, player->GetWorldTransform()->translation_.y, 0.04f);
}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//triangle2->Draw(camera);

	//

	//sphere->Draw(camera);

	levelEditor->Draw(camera);
	ground->Draw(camera);

	//model->Draw(camera);
	//model2->Draw(camera);


	//sprite->Draw(camera);
	//sprite2->Draw(camera);

	//particle->Draw();
	//particle2->Draw();

	skydome->Draw(camera);
	player->Draw(camera);
	weapon->Draw(camera);
	for (Enemy* enemy : enemys) 
	{
		enemy->Draw(camera);
	}


	colliderManager_->Draw(camera);
}

void GamePlayScene::CheckAllCollisions()
{


	//コライダーのリストをクリア
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player.get());

	if (weapon->GetIsAttack() == true)
	{
		//攻撃中のみ
		colliderManager_->AddColliders(weapon.get());
	}
	for (Enemy* enemy : enemys) 
	{
		if(enemy->GetIsAlive() == true)
		{
			//生きているときのみ
			colliderManager_->AddColliders(enemy);
		}
	}
	colliderManager_->AddColliders(levelEditor);
	//当たり判定
	colliderManager_->ChackAllCollisions();


}

void GamePlayScene::SpawnEnemy(const Vector3& position)
{
	// 敵を発生させる
	Enemy* enemy = new Enemy();
	// 敵の初期化
	enemy->Initialize();
	enemy->SetPosition(position);

	// リストに登録
	enemys.push_back(enemy);
}