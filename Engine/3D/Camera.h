/**
*	@file Camera.h
*	@brief カメラクラスヘッダ
*/


#pragma once
#include"MathUtilty.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Transform.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列

	Matrix4x4 sview;       // ワールド → ビュー変換行列
	Matrix4x4 sprojection; // ビュー → プロジェクション変換行列

	Vector3 worldPosition;
};


/**
*   @class Camera
*	@brief カメラクラス
*/
class Camera
{
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


	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;

	EulerTransform transform;
	Matrix4x4 matView;
	Matrix4x4 matProjection;

	Matrix4x4 smatView;
	Matrix4x4 smatProjection;

	Matrix4x4 cameraMatrix;

#pragma endregion


public:

	Camera() = default;///< デフォルトコンストラクタ
	~Camera() = default; ///< デフォルトデストラクタ

	/// @brief 初期化処理
	void Initialize();

	/// @brief 定数バッファ生成処理
	void CreateConstBuffer();

	/// @brief 定数バッファのマッピング
	void Map();

	/// @brief 行列の更新処理
	void UpdateMatrix();

	/// @brief 行列を転送する処理
	void TransferMatrix();

	/// @brief ビュー行列を更新する処理
	void UpdateViewMatrix();

	/// @brief 射影行列を更新する処理
	void UpdateProjectionMatrix();

	/// @brief デバッグ用関数
	void CameraDebug();

private:


	WinApp* winApp;
	DirectXCommon* dxCommon;

	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;
	/// @brief コピー禁止のため、コピーコンストラクタを削除
	Camera(const Camera&) = delete;
	/// @brief コピー禁止のため、代入演算子を削除
	Camera& operator=(const Camera&) = delete;
};

static_assert(!std::is_copy_assignable_v<Camera>);