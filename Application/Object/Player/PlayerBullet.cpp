#include "PlayerBullet.h"

void PlayerBullet::Initialize(ModelData bulletData, Vector3 pos, const Vector3& velocity)
{
	bullet = bullet->Create(bulletData);
	bullet->worldTransform_.translation_ = pos;
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	bullet->Update();
	//座標に移動
	SumVector3(bullet->worldTransform_.translation_, velocity_);

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}


}

void PlayerBullet::Draw(ICamera* camera)
{
	bullet->Draw(camera);
}