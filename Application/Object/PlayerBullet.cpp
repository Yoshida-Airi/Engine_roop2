#include "PlayerBullet.h"

void PlayerBullet::Initialize(ModelData bulletData, Vector3 pos)
{
	bullet = bullet->Create(bulletData);
	bullet->worldTransform_.translation_ = pos;
}

void PlayerBullet::Update()
{
	bullet->Update();

}

void PlayerBullet::Draw(ICamera* camera)
{
	bullet->Draw(camera);
}
