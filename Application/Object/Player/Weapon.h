#pragma once
#include"GameObject.h"
class Weapon :public GameObject
{
public:
	void Initialize()override;
	void Update()override;
	void Draw(Camera* camera)override;

	Vector3 GetWorldPosition()override;
	AABB GetAABB()override;
	void OnCollision([[maybe_unused]] Collider* other)override;

	void SetParent(Model* model)
	{
		weaponModel->Parent(model);
	};

	void SetIsAttack(bool isAttack)
	{
		isAttack_ = isAttack;
	}
	bool GetIsAttack()
	{
		return isAttack_;
	}

private:
	std::unique_ptr<Model>weaponModel;
	std::vector<Model*>weaponModels;

	bool isAttack_ = false; //攻撃中かどうか : ture 攻撃中
};

