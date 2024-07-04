#pragma once
#include"Model.h"
#include"Collider.h"
#include"GameObject.h"

class Enemy : public GameObject
{
public:
	void Initialize(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(Camera* camera)override;

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:

	Model* enemyModel;

	

};

