#pragma once

#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

class FlyEnemy : public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;
	void SetPosition(Vector3 position)
	{
		enemyModel->GetWorldTransform()->translation_ = position;
		initialY = enemyModel->GetWorldTransform()->translation_.y;
	}

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

	//ゲッター

	bool GetIsAlive()
	{
		return isAlive;
	}

private:

	std::unique_ptr<Model>enemyModel;
	std::vector<Model*>enemyModels;

	bool isAlive = true;	//生きているか: true 生きている

	float moveSpeed = 0.01f;  // 移動速度
	float moveDistance = 5.0f;  // 移動する距離
	float traveledDistance = 0.0f;  // 移動した距離
	bool movingRight = true;  // 右方向に移動しているかどうか

	float Amplitude = 1.0f;  // 浮く高さを1.0fに設定
	float Speed = 0.05f;      // 浮く速度をゆっくりとした0.1fに設定
	float initialY;  // 現在のy座標を初期値に設定
	float time = 0.0f;            // 時間の初期値を0に設定
};

