#include "Enemy.h"

void Enemy::Initialize()
{
	enemyModel.reset(Model::Create("Resources/SampleAssets/bunny.obj"));
}

void Enemy::Update()
{
	enemyModel->Update();
}

void Enemy::Draw(Camera* camera)
{
	enemyModel->Draw(camera);
}
