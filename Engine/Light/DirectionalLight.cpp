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
	ImGui::Begin("light");

	float direction[] = { lightData_->direction.x,lightData_->direction.y,lightData_->direction.z };
	ImGui::SliderFloat3("lightDirection", direction, -1.0f, 1.0f);

	lightData_->direction.x = direction[0];
	lightData_->direction.y = direction[1];
	lightData_->direction.z = direction[2];

	ImGui::End();
}

void DirectionalLight::Draw()
{
}


void DirectionalLight::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}