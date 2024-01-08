#include "Enemy.h"
#include"Player.h"

Enemy::~Enemy()
{
	delete enemy_;

	for (EnemyBullet* bullet : this->bullets_)
	{
		delete bullet;
	}
}

void Enemy::Initialize(ICamera* camera, ModelData model,ModelData bullet, const Vector3& position, const Vector3& velocityA, const Vector3& velocityB)
{
	camera_ = camera;
	enemyData_ = model;
	bulletData_ = bullet;
	enemy_ = new Model();
	enemy_->Initialize(enemyData_);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	//初期座標の設定
	enemy_->worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	ApprochVelocity_ = velocityA;
	LeaveVelocity_ = velocityB;
	enemy_->Initialize(model);

	timer = kFireInterval;

}

void Enemy::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->isDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});

	// 状態遷移
	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		Approach();

		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}


	enemy_->Update();

	//弾更新
	for (EnemyBullet* bullet : this->bullets_)
	{
		bullet->Update();
	}

#ifdef _DEBUG
	ImGui::Begin("enemy");

	float translate[3] = { enemy_->worldTransform_.translation_.x,enemy_->worldTransform_.translation_.y,enemy_->worldTransform_.translation_.z };
	ImGui::SliderFloat3("transform", translate, -10, 4);

	enemy_->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	ImGui::End();

#endif // _DEBUG

}

void Enemy::Draw()
{
	enemy_->Draw(camera_);

	//弾描画
	for (EnemyBullet* bullet : this->bullets_)
	{
		bullet->Draw();
	}

}

Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemy_->worldTransform_.matWorld_.m[3][0];
	worldpos.y = enemy_->worldTransform_.matWorld_.m[3][1];
	worldpos.z = enemy_->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Enemy::OnCollision()
{
}

// 接近フェーズ
void Enemy::Approach() {
	// 移動
	SumVector3(enemy_->worldTransform_.translation_, ApprochVelocity_);
	if (enemy_->worldTransform_.translation_.z <= 5.0f) {
		phase_ = Phase::Leave;
	}


	// 発射タイマーカウントダウン
	timer--;
	if (timer == 0)
	{
		//弾を発射
		Fire();
		//発射タイマーを初期化
		timer = kFireInterval;
	}

}

// 離脱フェーズ
void Enemy::Leave() {
	// 移動(ベクトルを加算)
	SumVector3(enemy_->worldTransform_.translation_, LeaveVelocity_);
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 0.5f;
	// 自キャラのワールド座標を取得する
	Vector3 worldPlayer = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 worldEnemy = GetWorldPosition();
	
	// 敵キャラ→自キャラの差分ベクトルを求める
	Vector3 differenceVector;
	differenceVector.x = worldPlayer.x - worldEnemy.x;
	differenceVector.y = worldPlayer.y - worldEnemy.y;
	differenceVector.z = worldPlayer.z - worldEnemy.z;

	// 正規化
	Vector3 normarizeVector;
	normarizeVector = Normalize(differenceVector);

	// ベクトルの長さを速さに合わせる
	Vector3 velocity;
	velocity.x = normarizeVector.x * kBulletSpeed;
	velocity.y = normarizeVector.y * kBulletSpeed;
	velocity.z = normarizeVector.z * kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(camera_,bulletData_, enemy_->worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(newBullet);
}

