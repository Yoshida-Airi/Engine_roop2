#include "RailCamera.h"

RailCamera::~RailCamera()
{
	delete camera;
}


// 角度をラジアンに変換する関数
float RailCamera::toRadians(float degrees) {
	return degrees * static_cast<float>(M_PI) / 180.0f;
}

// 円を描くための制御点を円上に等間隔で配置する関数
void RailCamera::setCircleControlPoints(std::vector<Vector3>& points, float radius, float centerX, float centerY, float centerZ) {
	

	// 360度を6等分して制御点を配置する
	for (int i = 0; i < 6; ++i) {
		float angle = toRadians(static_cast<float>(i * 60));  // 60度ごとに配置
		float x = centerX + radius * std::cos(angle);
		float y = centerY;
		float z= centerZ + radius * std::sin(angle);
		

		// 制御点の座標を追加
		points.push_back({ x, y, z });

	}
}

/// <summary>
/// 初期化
/// </summary>
void RailCamera::Initialize(WorldTransform worldTransform, Vector3& radian)
{
	//ワールドトランスフォームの初期設定
	worldTransform_ = worldTransform;
	worldTransform_.rotation_ = radian;

	//worldTransform_.translation_.z = -60;
	//ビュープロジェクションの初期化
	camera = new Camera();
	camera->farZ = 1000;
	camera->Initialize();

	setCircleControlPoints(controlPoints_, 0.1f, 0.0f, 0.0f, 0.0f);

	//controlPoints_ =
	//{
	//	{0,0,0},
	//	{0,0,0},
	//	{0,0,0},
	//	{0,0,0},
	//	{0,0,0},
	//	{0,0,0},
	//};

}

/// <summary>
/// 更新
/// </summary>
void RailCamera::Update()
{
	//視点の媒介変数の処理
	if (targetSection_ != 5) {
		eyet_ += 1.0f / 360.0f;
		if (eyet_ >= 1.0f) {
			eyet_ = 0.0f;
			eyeSection_++;
		}
	}

	//注視点の媒介変数の処理
	if (targetSection_ != 5) {
		targett_ += 1.0f / 360.0f;
		if (targett_ >= 1.0f) {
			targett_ = 0.0f;
			targetSection_++;
		}
	}

	eye_ = ClosedLoopCatmullRomPosition(controlPoints_, eyet_);
	target_ = ClosedLoopCatmullRomPosition(controlPoints_, targett_);


	//注視点と視点の差分ベクトル
	Vector3 forward = Normalize(Subtract(target_, eye_));

	//z座標軸
	Vector3 z = Subtract(target_, eye_);
	z = Normalize(z);
	//x座標軸
	Vector3 x = Cross({ 0.0f, 1.0f, 0.0f }, z);
	x = Normalize(x);
	//y座標軸
	Vector3 y = Cross(z, x);
	//view行列の作成
	Matrix4x4 matrix{};
	matrix.m[0][0] = x.x;
	matrix.m[0][1] = y.x;
	matrix.m[0][2] = z.x;
	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = x.y;
	matrix.m[1][1] = y.y;
	matrix.m[1][2] = z.y;
	matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = x.z;
	matrix.m[2][1] = y.z;
	matrix.m[2][2] = z.z;
	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -Dot(eye_, x);
	matrix.m[3][1] = -Dot(eye_, y);
	matrix.m[3][2] = -Dot(eye_, z);
	matrix.m[3][3] = 1.0f;
	camera->matView = matrix;

	//view行列の位置や方向をワールド行列に反映
	worldTransform_.translation_ = eye_;
	worldTransform_.rotation_.y = std::atan2(forward.x, forward.z);
	float length = Length({ forward.x, 0, forward.z });
	worldTransform_.rotation_.x = std::atan2(-forward.y, length);
	worldTransform_.matWorld_ = MakeAffinMatrix(
		worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);


	ImGui::Begin("RailCamera34");
	ImGui::DragFloat3("CameraTranslation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat4("WorldTransform m[0]", &worldTransform_.matWorld_.m[0][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[1]", &worldTransform_.matWorld_.m[1][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[2]", &worldTransform_.matWorld_.m[2][0], 0.01f);
	ImGui::DragFloat4("WorldTransform m[3]", &worldTransform_.matWorld_.m[3][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[0]", &camera->matView.m[0][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[1]", &camera->matView.m[1][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[2]", &camera->matView.m[2][0], 0.01f);
	ImGui::DragFloat4("ViewProjection m[3]", &camera->matView.m[3][0], 0.01f);
	ImGui::End();

}