#include "Weapon.h"
#include"Object/CollisionConfig.h"

void Weapon::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kWeapon);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	weaponModel.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	weaponModels = { weaponModel.get() };
	GameObject::Initialize();
	GameObject::SetModel(weaponModels);

	weaponModel->GetWorldTransform()->translation_.z += 2.0f;
	weaponModel->SetMaterial({ 0.0f,0.0,1.0f,1.0f });
}

void Weapon::Update()
{
	GameObject::Update();

	weaponModel->ModelDebug("weapon");
	//weaponModel->GetWorldTransform()->translation_.x += 0.03f;
}

void Weapon::Draw(Camera* camera)
{
	if (isAttack_ == false)
	{
		//攻撃してないときは武器を表示しない
		weaponModel->SetisInvisible(true);
	}
	else
	{
		weaponModel->SetisInvisible(false);
	}
	weaponModel->Draw(camera);
}

Vector3 Weapon::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = weaponModel->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = weaponModel->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = weaponModel->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Weapon::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - weaponModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - weaponModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - weaponModel->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + weaponModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + weaponModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + weaponModel->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Weapon::OnCollision(Collider* other)
{
}
