#include "Enemy.h"
#include"Player.h"
#include"GamePlayScene.h"

Enemy::~Enemy()
{
	delete enemy_;

	
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
	enemy_->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	//引数で受け取った速度をメンバ変数に代入
	ApprochVelocity_ = velocityA;
	LeaveVelocity_ = velocityB;
	enemy_->Initialize(model);

	timer = kFireInterval;



}

void Enemy::Update()
{

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

	

	if (Alive == true)
	{
		enemy_->Update();
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
	if (Alive == true)
	{
		enemy_->Draw(camera_);
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
	Alive = false;
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

	if (enemy_->worldTransform_.translation_.x <= -15)
	{
		enemy_->worldTransform_.translation_ = { 0.0f,0.2f,30.0f };
		phase_ = Phase::Approach;

		if (Alive == false)
		{
			Alive = true;
		}
	}


}

// 離脱フェーズ
void Enemy::Leave() {
	// 移動(ベクトルを加算)
	SumVector3(enemy_->worldTransform_.translation_, LeaveVelocity_);

	if (enemy_->worldTransform_.translation_.x <= -10)
	{
		enemy_->worldTransform_.translation_ = { 0.0f,0.2f,30.0f };
		phase_ = Phase::Approach;

		if (Alive == false )
		{
			Alive = true;
		}
	}


}

void Enemy::Fire()
{
	if (Alive == true)
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
		newBullet->Initialize(camera_, bulletData_, enemy_->worldTransform_.translation_, velocity);

		//弾を登録する
		gameScene_->AddEnemyBullet(newBullet);
	}
}

