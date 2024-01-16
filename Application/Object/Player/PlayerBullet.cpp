#include "PlayerBullet.h"

void PlayerBullet::Initialize(Vector3 pos, const Vector3& velocity)
{
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
