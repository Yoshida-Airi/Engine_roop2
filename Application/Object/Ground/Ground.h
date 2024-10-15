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

	[[maybe_unused]] void OnCollision([[maybe_unused]] Collider* other)override;

	void Debug(const char* name);

	void SetPosition(Vector3 position)
	{
		groundModel->GetWorldTransform()->translation_ = position;
	}

	void SetScale(Vector3 scale)
	{
		groundModel->GetWorldTransform()->scale_ = scale;
	}

	Vector3 GetScale()
	{
		return groundModel->GetWorldTransform()->scale_;
	}

private:
	


	std::unique_ptr<Model>groundModel = nullptr;

};

