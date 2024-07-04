#pragma once

#include"Model.h"
#include"Collider.h"

class GameObject : public Collider
{
protected:
	std::vector<Model*>models_;

public:
	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update();
	virtual void Draw(Camera* camera);

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() override;
	//AABB取得
	virtual AABB GetAABB() override;
	//衝突時に呼ばれる関数
	virtual void OnCollision([[maybe_unused]] Collider* other) override;

};

