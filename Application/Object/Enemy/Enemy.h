#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

class Enemy : public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;
	void SetPosition(Vector3 position)
	{
		enemyModel->GetWorldTransform()->translation_ = position;
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
	

};

