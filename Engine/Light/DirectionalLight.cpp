#include "DirectionalLight.h"

void DirectionalLight::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	LightBuffer();

	//ライトのデフォルト値
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;
	lightData_->active = true;
}

void DirectionalLight::Update()
{
}

void DirectionalLight::Draw()
{
}


void DirectionalLight::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}