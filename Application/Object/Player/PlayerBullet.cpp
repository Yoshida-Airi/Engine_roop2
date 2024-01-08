#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet()
{
	delete playerBullet_;
}

void PlayerBullet::Initialize(ICamera* camera, ModelData modelData, const Vector3& position, const Vector3& velocity)
{
	camera_ = camera;
	velocity_ = velocity;
	

	playerBullet_ = new Model();
	playerBullet_->Initialize(modelData);
	playerBullet_->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };

	// 引数で受け取った初期座標をセット
	playerBullet_->worldTransform_.translation_ = position;

}

void PlayerBullet::Update()
{

	//座標移動
	SumVector3(playerBullet_->worldTransform_.translation_, velocity_);


	playerBullet_->Update();

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

#ifdef _DEBUG
	//ImGui::Begin("player");

	//float translate[3] = { playerBullet_->worldTransform_.translation_.x,playerBullet_->worldTransform_.translation_.y,playerBullet_->worldTransform_.translation_.z };
	//ImGui::SliderFloat3("transform", translate, -10, 4);

	//playerBullet_->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	//ImGui::End();

#endif // _DEBUG

}

void PlayerBullet::Draw()
{
	playerBullet_->Draw(camera_);
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
	worldpos.x = playerBullet_->worldTransform_.matWorld_.m[3][0];
	worldpos.y = playerBullet_->worldTransform_.matWorld_.m[3][1];
	worldpos.z = playerBullet_->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}