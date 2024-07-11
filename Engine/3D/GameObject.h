#pragma once

#include"Model.h"
#include"Collider.h"

class GameObject : public Collider
{
protected:
	std::vector<Model*>models_;

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Draw(Camera* camera);
	virtual void SetModel(const std::vector<Model*>& models)
	{
		models_ = models;
	}

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() override = 0;
	//AABB取得
	virtual AABB GetAABB() override = 0;
	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) override = 0;

};

