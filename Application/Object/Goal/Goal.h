#pragma once
#include"GameObject.h"
#include"Object/CollisionConfig.h"
#include"Model.h"

class Goal :public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;

	Vector3 GetWorldPosition()override;

	AABB GetAABB()override;

	void OnCollision(Collider* other)override;

private:
	std::unique_ptr<Model>goalModel;
	std::vector<Model*>goalModels;
};