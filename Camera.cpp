//#include "Camera.h"
//
//void Camera::Initialize()
//{
//	winApp = WinApp::GetInstance();
//
//	transform =
//	{
//		{1.0f,1.0f,1.0f},
//		{0.0f,0.0f,0.0f},
//		{0.0f,0.0f,-5.0f}
//	};
//
//	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45, winApp->kCilentWidth / winApp->kCilentHeight, 0.1f, 100.0f);
//	Matrix4x4 matrix = MakeAffinMatrix(transform.scale, transform.rotate, transform.translate);
//	Matrix4x4 viewMatrix = Inverse(matrix);
//	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, winApp->kCilentWidth / winApp->kCilentHeight, 0.1f, 100.0f);
//
//}
