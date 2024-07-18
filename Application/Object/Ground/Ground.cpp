#include "Ground.h"

void Ground::Initialize()
{
	groundModel.reset(Model::Create("Resources/Level/Map.obj"));

	groundModel->GetWorldTransform()->translation_.x = -10.0f;
	groundModel->GetWorldTransform()->translation_.y = 3.0f;


}

void Ground::Update()
{
	groundModel->Update();
}

void Ground::Draw(Camera* camera)
{
	groundModel->Draw(camera);
}

Vector3 Ground::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = groundModel->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = groundModel->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = groundModel->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Ground::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - groundModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - groundModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - groundModel->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + groundModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + groundModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + groundModel->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Ground::OnCollision(Collider* other)
{
}

