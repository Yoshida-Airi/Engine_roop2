#include "CameraController.h"
#include"Object/Player/Player.h"
CameraController::~CameraController()
{
	
}
void CameraController::Initialize(Camera* camera)
{
	globalVariables = GlobalVariables::GetInstance();
	groupName = "Camera";

	camera_ = camera;
	camera_->Initialize();


	globalVariables->AddItem(groupName, "targetOffset", targetOffset_);
	globalVariables->AddItem(groupName, "imterpolationRate", interpolationRate);
}

void CameraController::Update()
{
	ApplyGlobalVariables();

	//追従対象のワールドトランスフォームを参照
	const WorldTransform* targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの目標座標を計算
	targetPosition = Add(targetWorldTransform->translation_, targetOffset_);

	//座標補間
	camera_->transform.translate = targetPosition;/*Lerp(camera_->transform.translate, targetPosition, interpolationRate);*/

	camera_->UpdateMatrix();
}

void CameraController::Reset()
{
	//追従対象のワールドトランスフォームを参照
	const WorldTransform* targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	camera_->transform.translate = Add(targetWorldTransform->translation_ , targetOffset_);


}

void CameraController::ApplyGlobalVariables()
{
	targetOffset_ = globalVariables->GetVector3Value(groupName, "targetOffset");
	interpolationRate = globalVariables->GetFloatValue(groupName, "imterpolationRate");
	

}
