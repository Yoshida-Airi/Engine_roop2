#include "UICamera.h"

void UICamera::Initialize()
{
	winApp = WinApp::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	transform =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-50.0f}
	};

	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void UICamera::CreateConstBuffer()
{
	constBuffer_ = dxCommon->CreateBufferResource(sizeof(ConstBufferUIDataViewProjection));
}

void UICamera::Map() {
	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void UICamera::UpdateMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void UICamera::TransferMatrix()
{
	constMap->view = matView;
	constMap->projection = matProjection;

}

void UICamera::UpdateViewMatrix()
{
	matView = MakeIdentity4x4();
}

void UICamera::UpdateProjectionMatrix()
{
	matProjection = MakeOrthographicmatrix(0.0f, 0.0f, float(winApp->kCilentWidth), float(winApp->kCilentHeight), 0.0f, 100.0f);
}

void UICamera::CameraDebug()
{
	ImGui::Begin("camera");

	float translate[3] = { transform.translate.x,transform.translate.y,transform.translate.z };
	ImGui::SliderFloat3("transform", translate, -10, 4);

	transform.translate = { translate[0],translate[1],translate[2] };

	UpdateMatrix();

	ImGui::End();
}
