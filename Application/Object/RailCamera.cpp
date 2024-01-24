#include "RailCamera.h"

RailCamera::~RailCamera()
{
	delete camera;
}

/// <summary>
/// 初期化
/// </summary>
void RailCamera::Initialize(WorldTransform worldTransform, Vector3& radian)
{
	//ワールドトランスフォームの初期設定
	worldTransform_ = worldTransform;
	worldTransform_.rotation_ = radian;

	worldTransform_.translation_.z = -60;
	//ビュープロジェクションの初期化
	camera = new Camera();
	camera->farZ = 1000;
	camera->Initialize();

}

/// <summary>
/// 更新
/// </summary>
void RailCamera::Update()
{
	//ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_.z += 0.03f;
	//ワールドトランスフォームの角度の数値を加算したりする(回転)
	/*worldTransform_.rotation_.x += 0.001f;*/
	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateWorldMatrix();
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	camera->matView = Inverse(worldTransform_.matWorld_);


	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	float CameraPos[] = {
		worldTransform_.translation_.x, worldTransform_.translation_.y,
		worldTransform_.translation_.z };

	ImGui::SliderFloat3("position", CameraPos, 30.0f, -30.0f);

	worldTransform_.translation_.x = CameraPos[0];
	worldTransform_.translation_.y = CameraPos[1];
	worldTransform_.translation_.z = CameraPos[2];

	float CameraRotate[] = {
		worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z };

	//カメラの角度
	ImGui::SliderFloat3("Rotate", CameraRotate, 30.0f, -30.0f);

	worldTransform_.rotation_.x = CameraRotate[0];
	worldTransform_.rotation_.y = CameraRotate[1];
	worldTransform_.rotation_.z = CameraRotate[2];

	ImGui::End();

}