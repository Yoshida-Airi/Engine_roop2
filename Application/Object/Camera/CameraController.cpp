#include "CameraController.h"
#include"Object/Player/Player.h"

void CameraController::Initialize()
{
}

void CameraController::Update()
{
	//追従対象とオフセットからカメラの座標を計算
	camera_->transform.translate = Add(target_->GetWorldTransform()->translation_, targetOffset_);
	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	camera_->transform.translate = Add(target_->GetWorldTransform()->translation_, targetOffset_);
}
