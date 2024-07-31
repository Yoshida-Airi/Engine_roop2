#include "Goal.h"

void Goal::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kGoal);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	goalModel.reset(Model::Create("Resources/Object/Goal/goal.obj"));
	goalModel->GetWorldTransform()->translation_.x = 39.0f;
	goalModel->GetWorldTransform()->translation_.y = -1.0f;

	goalModels = { goalModel.get() };

	GameObject::Initialize();
	GameObject::SetModel(goalModels);
}

void Goal::Update()
{
	GameObject::Update();

	goalModel->Update();

	goalModel->ModelDebug("goal");
}

void Goal::Draw(Camera* camera)
{
	goalModel->Draw(camera);
}

Vector3 Goal::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = goalModel->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = goalModel->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = goalModel->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Goal::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - goalModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - goalModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - goalModel->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + goalModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + goalModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + goalModel->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Goal::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		
	}
}