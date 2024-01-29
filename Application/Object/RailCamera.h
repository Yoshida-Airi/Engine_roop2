#pragma once
#include"WorldTransform.h"
#include"Camera.h"
#include"MathUtilty.h"
#include"ImGuiManager.h"
#include"Model.h"
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera
{
public:
	~RailCamera();

	float toRadians(float degrees);

	void setCircleControlPoints(std::vector<Vector3>& points, float radius, float centerX, float centerY, float centerZ, float elevationPerRotation);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform worldTransform, Vector3& radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションのゲッター
	/// </summary>
	/// <returns>レールカメラのビュープロジェクション</returns>
	Camera* GetCamera() { return camera; };

	/// <summary>
	/// トランスフォームのゲッター
	/// </summary>
	/// <returns>レールカメラのワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform_; };


private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	Camera* camera;

	std::vector<Vector3>controlPoints_;
	std::vector<Model>pointDrawing;
	const size_t segmentCount = 100;

	Vector3 eye_;
	Vector3 target_;
	float eyet_ = 0.0f;
	float targett_ = 0.1f;
	uint32_t eyeSection_ = 0;
	uint32_t targetSection_ = 0;

};