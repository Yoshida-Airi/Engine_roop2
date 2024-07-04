#include "GameObject.h"

void GameObject::Initialize(const std::vector<Model*>& models)
{
	Collider::Initialize();
	models_ = models;
}

void GameObject::Update()
{
	for (Model* model : models_)
	{
		model->Update();
	}
}

void GameObject::Draw(Camera* camera)
{
	for (Model* model : models_)
	{
		model->Draw(camera);
	}
}

Vector3 GameObject::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = models_[0]->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = models_[0]->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = models_[0]->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB GameObject::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - models_[0]->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - models_[0]->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - models_[0]->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + models_[0]->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + models_[0]->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + models_[0]->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void GameObject::OnCollision(Collider* other)
{
}
