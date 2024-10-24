#include "Boss.h"
#include"Object/CollisionConfig.h"

void Boss::Initialize()
{
	Collider::SetTypeID(CollisionTypeDef::kEnemy);
	Collider::SetColliderTypeID(ColliderType::SPHERE);
	Collider::SetRadius({1.0f,1.0f,1.0f});

	enemyModel_.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	enemyModels_ = { enemyModel_.get() };
	GameObject::Initialize();
	GameObject::SetModel(enemyModels_);
	enemyModel_->GetWorldTransform()->scale_ = { 2.0f, 2.0f, 2.0f };
	enemyModel_->SetMaterial({ 1.0f,0.0,0.0f,1.0f });
	enemyModel_->GetWorldTransform()->translation_.y = 2.0f;

}

void Boss::Update()
{
	GameObject::Update();

	enemyModel_->ModelDebug("enemy");

#ifdef _DEBUG
	ImGui::Text("BossHP %d", hp_);
#endif // _DEBUG


	//enemyModel->GetWorldTransform()->translation_.x += 0.03f;

	// 移動距離を更新
	if (movingRight_) {
		enemyModel_->GetWorldTransform()->translation_.x += moveSpeed_;
		traveledDistance_ += moveSpeed_;
	}
	else {
		enemyModel_->GetWorldTransform()->translation_.x -= moveSpeed_;
		traveledDistance_ -= moveSpeed_;
	}

	// 一定の距離を移動したら方向を反転
	if (traveledDistance_ >= moveDistance_) {
		movingRight_ = false;
	}
	else if (traveledDistance_ <= 0.0f) {
		movingRight_ = true;
	}

	if (hp_ <= 0)
	{
		isAlive_ = false;
	}

	//無敵時間
	if (isInvincible_)
	{
		invincibilityTimer_ -= 1.0f / 60.0f;
		if (invincibilityTimer_ <= 0.0f)
		{
			isInvincible_ = false;
		}
	}

}

void Boss::Draw(Camera* camera)
{
	if (isAlive_ == false)
	{
		//死んでいたら描画しない
		enemyModel_->SetisInvisible(true);
	}

	enemyModel_->Draw(camera);
}


Vector3 Boss::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = enemyModel_->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = enemyModel_->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = enemyModel_->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Boss::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - enemyModel_->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + enemyModel_->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + enemyModel_->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + enemyModel_->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Boss::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kWeapon))
	{
		if (!isInvincible_)
		{
			hp_ -= 1;
			isInvincible_ = true;
			invincibilityTimer_ = invincibilityDuration_;
		}
	}
}