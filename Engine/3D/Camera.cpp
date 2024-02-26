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
#ifdef _DEBUG
	ImGui::Begin("camera");

	float translate[3] = { transform.translate.x,transform.translate.y,transform.translate.z };
	ImGui::DragFloat3("transform", translate, 0.01f);
	float rotate[3] = { transform.rotate.x,transform.rotate.y,transform.rotate.z };
	ImGui::DragFloat3("rotate", rotate, 0.01f);
	//float scale[3] = { transform.scale.x, transform.scale.y, transform.scale.z };
	//ImGui::DragFloat3("scale", scale, 1);


	transform.translate = { translate[0],translate[1],translate[2] };
	transform.rotate = { rotate[0],rotate[1],rotate[2] };
	//transform.scale = { scale[0],scale[1],scale[2] };

	UpdateMatrix();

	ImGui::End();

#endif // _DEBUG



	
}
