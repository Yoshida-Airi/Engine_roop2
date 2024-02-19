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

	constMap->sview = smatView;
	constMap->sprojection = smatProjection;

	constMap->worldPosition = transform.translate;
}

void Camera::UpdateViewMatrix()
{
	cameraMatrix = MakeAffinMatrix(transform.scale, transform.rotate, transform.translate);
	matView = Inverse(cameraMatrix);
	smatView = MakeIdentity4x4();
}

void Camera::UpdateProjectionMatrix()
{
	matProjection = MakePerspectiveFovMatrix(0.45f, float(winApp->kCilentWidth) / float(winApp->kCilentHeight), nearZ, farZ);
	smatProjection = MakeOrthographicmatrix(0.0f, 0.0f, float(winApp->kCilentWidth), float(winApp->kCilentHeight), 0.0f, 100.0f);
}

void Camera::CameraDebug()
{

	ImGui::Begin("camera");

	float translate[3] = { transform.translate.x,transform.translate.y,transform.translate.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);
	float rotate[3] = { transform.rotate.x,transform.rotate.y,transform.rotate.z };
	ImGui::SliderFloat3("rotate", rotate, -20, 4);

	transform.translate = { translate[0],translate[1],translate[2] };
	transform.rotate = { rotate[0],rotate[1],rotate[2] };

	UpdateMatrix();

	ImGui::End();


	
}
