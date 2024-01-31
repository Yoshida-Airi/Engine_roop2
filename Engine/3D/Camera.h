#pragma once
#include"ICamera.h"
#include"MathUtilty.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列

};


/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
class Camera :public ICamera {
public:

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = 45.0f * 3.141592654f / 180.0f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;

#pragma endregion



	Camera() = default;
	~Camera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer()override;
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map()override;
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix()override;
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix()override;
	/// <summary>
	/// ビュー行列を更新する
	/// </summary>
	void UpdateViewMatrix()override;
	/// <summary>
	/// 射影行列を更新する
	/// </summary>
	void UpdateProjectionMatrix()override;

	void CameraDebug()override;

private:

	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;
	// コピー禁止
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
};

static_assert(!std::is_copy_assignable_v<Camera>);