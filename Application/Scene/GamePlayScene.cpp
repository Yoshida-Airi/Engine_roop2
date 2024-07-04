#include "GamePlayScene.h"

GamePlayScene::~GamePlayScene()
{

	delete camera;
	delete levelEditor;

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

	playerModel.reset(Model::Create("Resources/Object/Player/player.obj"));
	playerModels = { playerModel.get() };

	enemyModel.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels = { enemyModel.get() };

	camera = new Camera;
	camera->Initialize();

	levelEditor = new LevelEditor();
	levelEditor->LoaderJsonFile();
	levelEditor->Initialize(playerModels);

	player = std::make_unique<Player>();
	player->Initialize(playerModels);

	enemy = std::make_unique<Enemy>();
	enemy->Initialize(enemyModels);

	skydome = std::make_unique<Skydome>();
	skydome->Initialize();



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

	player->Update();
	enemy->Update();
	skydome->Update();

	CheckAllCollisions();

	camera->transform.translate.x = LerpShortTranslate(camera->transform.translate.x, player->GetWorldTransform()->translation_.x, 0.04f);

}

void GamePlayScene::Draw()
{

	//triangle->Draw(camera);
	//triangle2->Draw(camera);

	//

	//sphere->Draw(camera);

	levelEditor->Draw(camera);

	//model->Draw(camera);
	//model2->Draw(camera);


	//sprite->Draw(camera);
	//sprite2->Draw(camera);

	//particle->Draw();
	//particle2->Draw();

	skydome->Draw(camera);
	player->Draw(camera);
	enemy->Draw(camera);

	colliderManager_->Draw(camera);
}

void GamePlayScene::CheckAllCollisions()
{


	//コライダーのリストをクリア
	colliderManager_->ListClear();

	//コライダーにオブジェクトを登録
	colliderManager_->AddColliders(player.get());
	colliderManager_->AddColliders(enemy.get());
	colliderManager_->AddColliders(levelEditor);
	//当たり判定
	colliderManager_->ChackAllCollisions();


}
