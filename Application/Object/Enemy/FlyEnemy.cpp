#include "FlyEnemy.h"

void FlyEnemy::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	enemyModel.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels = { enemyModel.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels);
	enemyModel->SetMaterial({ 1.0f,0.0,0.0f,1.0f });
}

void FlyEnemy::Update()
{
	GameObject::Update();

	enemyModel->ModelDebug("enemy");
	//enemyModel->GetWorldTransform()->translation_.x += 0.03f;

	// 移動距離を更新
	if (movingRight) {
		enemyModel->GetWorldTransform()->translation_.x += moveSpeed;
		traveledDistance += moveSpeed;
	}
	else {
		enemyModel->GetWorldTransform()->translation_.x -= moveSpeed;
		traveledDistance -= moveSpeed;
	}

	// 一定の距離を移動したら方向を反転
	if (traveledDistance >= moveDistance) {
		movingRight = false;
	}
	else if (traveledDistance <= 0.0f) {
		movingRight = true;
	}

}

void FlyEnemy::Draw(Camera* camera)
{
	if (isAlive == false)
	{
		//死んでいたら描画しない
		enemyModel->SetisInvisible(true);
	}

	enemyModel->Draw(camera);
}


Vector3 FlyEnemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB FlyEnemy::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void FlyEnemy::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		isAlive = false;
	}
}