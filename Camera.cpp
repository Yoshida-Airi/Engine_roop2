#include "Camera.h"
#include "Camera.h"


void Camera::Initialize()
{
	winApp = WinApp::GetInstance();
	dxCommon = DirectXCommon::GetInstance();

	transform =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,-10.0f}
	};

	
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void Camera::CreateConstBuffer()
{
	constBuffer_ = dxCommon->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void Camera::Map() {
	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void Camera::UpdateMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void Camera::TransferMatrix()
{
	constMap->view = matView;
	constMap->projection = matProjection;
}

void Camera::UpdateViewMatrix()
{
	Matrix4x4 cameraMatrix = MakeAffinMatrix(transform.scale, transform.rotate, transform.translate);
	matView = Inverse(cameraMatrix);
}

void Camera::UpdateProjectionMatrix()
{
	matProjection = MakePerspectiveFovMatrix(0.45f, float(winApp->kCilentWidth) / float(winApp->kCilentHeight), nearZ, farZ);
}

void Camera::cameraDebug()
{
	ImGui::Begin("camera");

	float translate[3] = { transform.translate.x,transform.translate.y,transform.translate.z };
	ImGui::SliderFloat3("transform", translate, -10, 0);

	transform.translate = { translate[0],translate[1],translate[2] };

	UpdateMatrix();
	UpdateViewMatrix();

	ImGui::End();
}
