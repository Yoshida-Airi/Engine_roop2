#include "EnemyBullet.h"

void EnemyBullet::Initialize(ModelData bulletData, Vector3 pos, const Vector3& velocity)
{
	bullet = bullet->Create(bulletData);
	bullet->worldTransform_.translation_ = pos;
	velocity_ = velocity;


	// Z方向に伸びた形状
	bullet->worldTransform_.scale_.x = 0.5f;
	bullet->worldTransform_.scale_.y = 0.5f;
	bullet->worldTransform_.scale_.z = 3.0f;

	

}

void EnemyBullet::Update()
{
	bullet->Update();
	//座標に移動
	SumVector3(bullet->worldTransform_.translation_, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

	//***********************************************//
	//			　　　弾を進行方向に向ける				 //
	//***********************************************//
	// Y軸周り角度(θy)
	bullet->worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float velocityXZ = sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	// X軸周りの角度(θx)
	bullet->worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);


}

void EnemyBullet::Draw(ICamera* camera)
{
	bullet->Draw(camera);
}
