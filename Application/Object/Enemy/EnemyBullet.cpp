#include "EnemyBullet.h"
#include "Player.h"
#include"CollisionConfig.h"

void EnemyBullet::Initialize(Vector3 pos, const Vector3& velocity)
{

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionAttributePlayer);

	bulletModel_ = Model::Create("Resources", "enemyBullet.obj");
	bulletModel_->worldTransform_->translation_ = pos;
	velocity_ = velocity;


	//// Z方向に伸びた形状
	//bulletModel_->worldTransform_->scale_.x = 0.5f;
	//bulletModel_->worldTransform_->scale_.y = 0.5f;
	//bulletModel_->worldTransform_->scale_.z = 3.0f;

}

void EnemyBullet::Update()
{
	
	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}


	//***********************************************//
	//					　ホーミング					 //
	//***********************************************//

	// 敵弾から自キャラへのベクトルを計算
	//Vector3 toPlayer = Subtract(player_->GetWorldPosition(), GetWorldPosition());

	//// ベクトルを正規化する
	//toPlayer = Normalize(toPlayer);
	//velocity_ = Normalize(velocity_);

	//// 球面線形補完により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	//velocity_ = Slerp(velocity_, toPlayer, 0.009f);
	//velocity_.x *= 2.0f;
	//velocity_.y *= 2.0f;
	//velocity_.z *= 2.0f;

	//***********************************************//
	//			　　　弾を進行方向に向ける				 //
	//***********************************************//
	// Y軸周り角度(θy)
	bulletModel_->worldTransform_->rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸周りの角度(θx)
	bulletModel_->worldTransform_->rotation_.x = std::atan2(-velocity_.y, velocityXZ);


	//座標に移動
	SumVector3(bulletModel_->worldTransform_->translation_, velocity_);
	bulletModel_->Update();


}

void EnemyBullet::Draw(ICamera* camera)
{
	bulletModel_->Draw(camera);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = bulletModel_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = bulletModel_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = bulletModel_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}
