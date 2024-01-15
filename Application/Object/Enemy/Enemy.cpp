#include "Enemy.h"

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_)
	{
		delete bullet;
	}
}

void Enemy::Initialize(const ModelData enemyData, const ModelData bulletData, Vector3 pos)
{
	enemyData_ = enemyData;
	bulletData_ = bulletData;


	enemy = Model::Create(enemyData);
	enemy->worldTransform_.translation_ = pos;

	state = new EnemyStateApproach();
	state->Initialize(this);

	fireTimer_ = kFireInterval;

}

void Enemy::Update()
{
	enemy->Update();

	state->Update(this);

	//弾を一定間隔に発射
	fireTimer_--;
	if (fireTimer_ == 0)
	{
		//攻撃処理
		Fire();
		fireTimer_ = kFireInterval;
	}

	//弾の更新
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	
}

void Enemy::Draw(ICamera* camera)
{
	enemy->Draw(camera);

	//弾の描画
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw(camera);
	}
}

void Enemy::Move(Vector3& velocity)
{
	SumVector3(enemy->worldTransform_.translation_, velocity);
}

void Enemy::ChangeState(IEnemyState* newState)
{
	state = newState;
}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemy->worldTransform_.matWorld_.m[3][0];
	worldpos.y = enemy->worldTransform_.matWorld_.m[3][1];
	worldpos.z = enemy->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Enemy::Fire()
{


	//ですフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});


	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転
	velocity = TransformNormal(velocity, enemy->worldTransform_.matWorld_);

	//球を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletData_, GetWorldPosition(), velocity);

	//弾の登録
	bullets_.push_back(newBullet);



}
