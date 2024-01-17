#include "Skydome.h"

void Skydome::Initialize()
{
	skydomeModel_ = Model::Create("Resources/Skydome", "skydome.obj");
	

	// 天球の大きさ(大きさによってはカメラのfarZをSetterで設定しよう)
	skydomeModel_->worldTransform_->scale_.x = 100.0f;
	skydomeModel_->worldTransform_->scale_.y = 100.0f;
	skydomeModel_->worldTransform_->scale_.z = 100.0f;


}

void Skydome::Update()
{
	skydomeModel_->Update();
}

void Skydome::Draw(ICamera* camera)
{
	skydomeModel_->Draw(camera);
}
