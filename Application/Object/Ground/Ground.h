#pragma once
#include"Model.h"
#include"GameObject.h"

class Ground :public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

private:
	


	std::unique_ptr<Model>groundModel = nullptr;

};

