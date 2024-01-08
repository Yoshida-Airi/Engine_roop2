#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet()
{
	delete enemyBullet_;
}

void EnemyBullet::Initialize(ICamera* camera, ModelData modelData, const Vector3& position, const Vector3& velocity)
{
	camera_ = camera;
	velocity_ = velocity;


	enemyBullet_ = new Model();
	enemyBullet_->Initialize(modelData);
	enemyBullet_->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };

	// 引数で受け取った初期座標をセット
	enemyBullet_->worldTransform_.translation_ = position;

}

void EnemyBullet::Update()
{

	//座標移動
	SumVector3(enemyBullet_->worldTransform_.translation_, velocity_);


	enemyBullet_->Update();


#ifdef _DEBUG
	//ImGui::Begin("player");

	//float translate[3] = { playerBullet_->worldTransform_.translation_.x,playerBullet_->worldTransform_.translation_.y,playerBullet_->worldTransform_.translation_.z };
	//ImGui::SliderFloat3("transform", translate, -10, 4);

	//playerBullet_->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	//ImGui::End();

#endif // _DEBUG

}

void EnemyBullet::Draw()
{
	enemyBullet_->Draw(camera_);
}
