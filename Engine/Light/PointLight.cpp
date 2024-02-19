#include "PointLight.h"

void PointLight::Initialize()
{
    dxCommon_ = DirectXCommon::GetInstance();
    lightResource_ = dxCommon_->CreateBufferResource(sizeof(PointLight));
    lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData));

    // Point lightの初期化処理を行う
    lightData.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f); // 白い光
    lightData.position =Vector3(1.0f, 1.0f, 1.0f);    // 光源の初期位置
    lightData.intensity = 1.0f;                             // 光の強度
}

void PointLight::Update()
{

#ifdef _DEBUG

	ImGui::Begin("light");

	float position[] = { lightData.position.x,lightData.position.y,lightData.position.z };
	ImGui::SliderFloat3("position", position, -1.0f, 1.0f);
	
	lightData.position.x = position[0];
	lightData.position.y = position[1];
	lightData.position.z = position[2];

	float color[] = { lightData.color.x, lightData.color.y, lightData.color.z, lightData.color.w };
	ImGui::ColorEdit4("Pick A Color", color);

	lightData.color.x = color[0];
	lightData.color.y = color[1];
	lightData.color.z = color[2];
	lightData.color.w = color[3];

	ImGui::End();
#endif // _DEBUG
}

void PointLight::Draw()
{
}
