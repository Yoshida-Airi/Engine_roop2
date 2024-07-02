#pragma once
#include"Model.h"
#include"Collider.h"


class Enemy : public Collider
{
public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:

	std::unique_ptr<Model>enemyModel;

	

};

