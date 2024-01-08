#include "Skydome.h"

Skydome::~Skydome()
{
	delete skydome_;
}

void Skydome::Initialize(ICamera* camera, ModelData skydome, const Vector3& position)
{
	camera_ = camera;

	skydome_ = new Model();
	skydome_->Initialize(skydome);


	skydome_->worldTransform_.translation_ = position;

	// 天球の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)
	skydome_->worldTransform_.scale_.x = 100.0f;
	skydome_->worldTransform_.scale_.y = 100.0f;
	skydome_->worldTransform_.scale_.z = 100.0f;

	skydome_->SetIsEnableLight(false);
}

void Skydome::Update()
{
	skydome_->Update();
}

void Skydome::Draw()
{
	skydome_->Draw(camera_);
}
