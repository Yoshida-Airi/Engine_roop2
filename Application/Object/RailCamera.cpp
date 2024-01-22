#include "RailCamera.h"

RailCamera::~RailCamera()
{
	delete camera;
}

void RailCamera::Initialize()
{
	cameraModel = Model::Create("Resources", "cube.obj");
	cameraModel->SetisInvisible(true);
	camera = new Camera();
	camera->Initialize();
}

void RailCamera::Update()
{
	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	cameraModel->worldTransform_->translation_.z -= 0.03f;
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
	/*worldTransform_.rotation_.x += 0.001f;*/
	//ワールドトランスフォームのワールド行列再計算
	cameraModel->worldTransform_->UpdateWorldMatrix();


	camera->matView = Inverse(cameraModel->worldTransform_->matWorld_);
	cameraModel->Update();




	//カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");
	float CameraPos[] = {
		cameraModel->worldTransform_->translation_.x, cameraModel->worldTransform_->translation_.y,
		cameraModel->worldTransform_->translation_.z };

	ImGui::SliderFloat3("position", CameraPos, 30.0f, -30.0f);

	cameraModel->worldTransform_->translation_.x = CameraPos[0];
	cameraModel->worldTransform_->translation_.y = CameraPos[1];
	cameraModel->worldTransform_->translation_.z = CameraPos[2];

	float CameraRotate[] = {
		cameraModel->worldTransform_->rotation_.x, cameraModel->worldTransform_->rotation_.y, cameraModel->worldTransform_->rotation_.z };

	//カメラの角度
	ImGui::SliderFloat3("Rotate", CameraRotate, 30.0f, -30.0f);

	cameraModel->worldTransform_->rotation_.x = CameraRotate[0];
	cameraModel->worldTransform_->rotation_.y = CameraRotate[1];
	cameraModel->worldTransform_->rotation_.z = CameraRotate[2];

	ImGui::End();

}

Vector3 RailCamera::GetWorldPosition()
{

	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = cameraModel->worldTransform_->matWorld_.m[3][0];
	worldpos.y = cameraModel->worldTransform_->matWorld_.m[3][1];
	worldpos.z = cameraModel->worldTransform_->matWorld_.m[3][2];

	return worldpos;

}
