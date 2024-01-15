#include "Enemy.h"

Enemy::~Enemy()
{
	for (EnemyBullet* bullet : bullets_)
	{
		delete bullet;
	}


	for (TimedCall* timeCall : this->timedCalls_)
	{
		delete timeCall;
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

}

void Enemy::Update()
{
	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timeCall) {
		if (timeCall->isFinished() == true) {
			delete timeCall;
			return true;
		}
		return false;
		});


	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});


	enemy->Update();

	state->Update(this);

	//タイマーの更新
	for (TimedCall* timedCall : this->timedCalls_)
	{
		timedCall->Update();
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

void Enemy::AttackReset()
{
	Fire();
	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::AttackReset, this), kFireInterval));
}

void Enemy::DeleteBullet()
{
	timedCalls_.clear();
}

void Enemy::Fire()
{

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
