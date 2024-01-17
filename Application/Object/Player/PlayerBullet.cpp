#include "PlayerBullet.h"
#include"CollisionConfig.h"

void PlayerBullet::Initialize(Vector3 pos, const Vector3& velocity)
{
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionAttributeEnemy);


	bulletModel_ = Model::Create("Resources","cube.obj");
	bulletModel_->worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	bulletModel_->Update();
	//座標に移動
	SumVector3(bulletModel_->worldTransform_.translation_, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}


}

void PlayerBullet::Draw(ICamera* camera)
{
	bulletModel_->Draw(camera);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = bulletModel_->worldTransform_.matWorld_.m[3][0];
	worldpos.y = bulletModel_->worldTransform_.matWorld_.m[3][1];
	worldpos.z = bulletModel_->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}