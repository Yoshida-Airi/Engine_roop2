#pragma once
#include"ICamera.h"
#include"Camera.h"
#include"Model.h"
class RailCamera
{
public:
	
	~RailCamera();

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

	
};

