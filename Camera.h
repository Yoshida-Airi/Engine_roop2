#pragma once
#include "MatrixMath.h"
#include "VectorMath.h"
#include"MathUtilty.h"
#include <d3d12.h>
#include <type_traits>
#include <wrl.h>
#include"DirectXCommon.h"
#include"WinApp.h"
#include"Transform.h"

// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列

	Matrix4x4 UIView;
	Matrix4x4 UIProjection;
};


/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
class Camera {
public:
#pragma region ビュー行列の設定

	Transform transform;

#pragma endregion

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

	// ビュー行列
	Matrix4x4 matView;
	// 射影行列
	Matrix4x4 matProjection;

	// ビュー行列
	Matrix4x4 UIMatView;
	// 射影行列
	Matrix4x4 UIMatProjection;

	Camera() = default;
	~Camera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();
	/// <summary>
	/// ビュー行列を更新する
	/// </summary>
	void UpdateViewMatrix();
	/// <summary>
	/// 射影行列を更新する
	/// </summary>
	void UpdateProjectionMatrix();
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns>定数バッファ</returns>
	
	void cameraDebug();


	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetConstBuffer() const { return constBuffer_; }

private:
	WinApp* winApp;
	DirectXCommon* dxCommon;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap = nullptr;
	// コピー禁止
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
};

static_assert(!std::is_copy_assignable_v<Camera>);