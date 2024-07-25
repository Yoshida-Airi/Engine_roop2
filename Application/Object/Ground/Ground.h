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

	void SetPosition(Vector3 position)
	{
		groundModel->GetWorldTransform()->translation_ = position;
	}

	void SetScale(Vector3 scale)
	{
		groundModel->GetWorldTransform()->scale_ = scale;
	}



private:
	


	std::unique_ptr<Model>groundModel = nullptr;

};

