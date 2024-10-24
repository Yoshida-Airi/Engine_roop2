#define NOMINMAX

#include "Player.h"
#include"Object/CollisionConfig.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"
#include"Object/Ground/MapChipField.h"

void Player::Initialize()
{
	grobalVariables = GlobalVariables::GetInstance();
	groupName = "Player";

	playerModel.reset(Model::Create("Resources/Object/Player/player.obj"));
	playerModels = { playerModel.get() };

	Collider::SetTypeID(CollisionTypeDef::kPlayer);
	Collider::SetColliderTypeID(ColliderType::SPHERE);

	GameObject::Initialize();
	GameObject::SetModel(playerModels);

	weapon_->SetParent(playerModel.get());

	//モデルの初期設定
	//playerModel->GetWorldTransform()->rotation_.y = std::numbers::pi_v<float> / 2.0f;
	playerModel->GetWorldTransform()->translation_.x = 7.2f;
	playerModel->GetWorldTransform()->translation_.y = 75.0f;
	//playerModel->GetWorldTransform()->rotation_.y = 1.5f;


	grobalVariables->AddItem(groupName, "Acceleration", kAcceleration);
	grobalVariables->AddItem(groupName, "Attenuation", kAttenuation);
	grobalVariables->AddItem(groupName, "LimitRunSpeed", kLimitRunSpeed);
	grobalVariables->AddItem(groupName, "GravityAcceleration", kGravityAcceleration);
	grobalVariables->AddItem(groupName, "LimitFallSpead", kLimitFallSpeed);
	grobalVariables->AddItem(groupName, "JumpAcceleration", kJumpAcceleration);
	grobalVariables->AddItem(groupName, "playerWidth", kWidth);
	grobalVariables->AddItem(groupName, "playerHeight", kHeight);




}

void Player::Update()
{
	GameObject::Update();
	ApplyGlobalVariables();
	//デバッグ
	playerModel->ModelDebug("player");


	BehaviorRootUpdate();
	BehaviorAttackUpdate();

	//速度加算
	//playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, velocity_);


	collisionMapInfo.move = velocity_;
	//マップ衝突チェック
	CollisionMap(collisionMapInfo);

	CollisionMove(collisionMapInfo);
	HitTop(collisionMapInfo);
	CollisionWall(collisionMapInfo);
	SwitchGround(collisionMapInfo);

#ifdef _DEBUG
	ImGui::Text("x %d", collisionMapInfo.move.x);
	ImGui::Text("y %d", collisionMapInfo.move.y);
	ImGui::Text("z %d", collisionMapInfo.move.z);
	ImGui::Text("isTop %d", collisionMapInfo.isTop);
	ImGui::Text("isGround %d", collisionMapInfo.isGround);
	ImGui::Text("isWall %d", collisionMapInfo.isWall);
	ImGui::Text("grand %d", onGround_);
	ImGui::Text("HP %d", HP);
#endif // _DEBUG

	//無敵時間
	if (isInvincible)
	{
		invincibilityTimer -= 1.0f / 60.0f;
		if (invincibilityTimer <= 0.0f)
		{
			isInvincible = false;
		}
	}

	if (HP == 0)
	{
		// アルファ値を段々と減少させる
		alpha -= fadeSpeed;
		if (alpha < 0.0f) {
			alpha = 0.0f;  // アルファ値が負にならないように制限
		}

		playerModel->SetMaterial({ 1.0f,1.0f,1.0f,alpha });
	}
}

void Player::Draw(Camera* camera)
{
	playerModel->Draw(camera);

}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = playerModel->GetWorldTransform()->matWorld_.m[3][0];
	worldpos.y = playerModel->GetWorldTransform()->matWorld_.m[3][1];
	worldpos.z = playerModel->GetWorldTransform()->matWorld_.m[3][2];

	return worldpos;
}

AABB Player::GetAABB()
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = { worldPos.x - playerModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y - playerModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z - playerModel->GetWorldTransform()->scale_.z / 2.0f };
	aabb.max = { worldPos.x + playerModel->GetWorldTransform()->scale_.x / 2.0f,worldPos.y + playerModel->GetWorldTransform()->scale_.y / 2.0f,worldPos.z + playerModel->GetWorldTransform()->scale_.z / 2.0f };

	return aabb;
}

void Player::OnCollision(Collider* other)
{
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kGoal))
	{
		hitGoal = true;
	}
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kEnemy))
	{
		if (!isInvincible)
		{
			HP -= 1;
			isInvincible = true;
			invincibilityTimer = invincibilityDuration;
		}
	}
}

void Player::Move()
{
	///* --プレイヤーの移動処理-- */

	velocity_ = Add(velocity_, { 0.0f, -kGravityAcceleration, 0.0f });
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	//移動処理
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) 
	{
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) 
		{
			//旋回処理
			if (lrDirection != LRDirection::kRight)
			{
				lrDirection = LRDirection::kRight;
				turnFirstRotationY = std::numbers::pi_v<float>;
				turnTimer = 1.0f;
			}
			//減速処理
			if (velocity_.x < 0.0f) 
			{
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		}
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
		{
			//旋回処理
			if (lrDirection != LRDirection::kLeft)
			{
				lrDirection = LRDirection::kLeft;
				turnFirstRotationY = 0.0f;
				turnTimer = 1.0f;
			}
			//減速処理
			if (velocity_.x > 0.0f) 
			{
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_ = Add(velocity_, acceleration);
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} // 非按键时速度衰减
	else 
	{
		velocity_.x *= (1.0f - kAttenuation);
	}


	//ジャンプ処理
	if (onGround_)
	{

		if (Input::GetInstance()->PushKey(DIK_UP)) 
		{
			if (!isJump)
			{
				velocity_ = Add(velocity_, { 0.0f, kJumpAcceleration, 0.0f });
				isJump = true;
			}
			else
			{
				isJump = false;
			}
		}
	}
	else//空中
	{
		//落下速度
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::Turn()
{
	/* --プレイヤーの旋回処理-- */

	if (turnTimer > 0.0f) 
	{
		turnTimer = std::clamp(turnTimer - 1 / 30.0f, 0.0f, turnTimer);
		float destinationRotationYTable[] = { 0, std::numbers::pi_v<float>, };
		//角度の取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection)];
		playerModel->GetWorldTransform()->rotation_.y = std::lerp(destinationRotationY, turnFirstRotationY, turnTimer);
	}
}

void Player::Jump()
{
	//接地状態
	if (onGround_)
	{
		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (!isJump)
			{
				//ジャンプ初速
				velocity_ = Add(velocity_, Vector3(0, kJumpAcceleration, 0));
				isJump = true;
			}
		}
		else
		{
			isJump = false;
		}



	}
	else//空中
	{
		//落下速度
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}


}

void Player::ApplyGlobalVariables()
{
	kAcceleration = grobalVariables->GetFloatValue(groupName, "Acceleration");
	kAttenuation = grobalVariables->GetFloatValue(groupName, "Attenuation");
	kLimitRunSpeed = grobalVariables->GetFloatValue(groupName, "LimitRunSpeed");
	kGravityAcceleration = grobalVariables->GetFloatValue(groupName, "GravityAcceleration");
	kLimitFallSpeed = grobalVariables->GetFloatValue(groupName, "LimitFallSpead");
	kJumpAcceleration = grobalVariables->GetFloatValue(groupName, "JumpAcceleration");
	kWidth = grobalVariables->GetFloatValue(groupName, "playerWidth");
	kHeight = grobalVariables->GetFloatValue(groupName, "playerHeight");

}


void Player::BehaviorRootUpdate()
{
	//移動処理
	Move();
	//旋回処理
	Turn();
	//ジャンプ処理
	//Jump();
}

void Player::BehaviorAttackUpdate()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		weapon_->SetIsAttack(true);
	}
	else
	{
		weapon_->SetIsAttack(false);
	}
}

void Player::CollisionMap(CollisionMapInfo& info)
{
	CollisionMapTop(info);
	CollisionMapBottom(info);
	CollisionMapLeft(info);
	CollisionMapRight(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info)
{
	if (info.move.y <= 0)
	{
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) 
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	//左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) 
	{
		hit = true;
	}

	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { 0.0f, kHeight / 2.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(playerModel->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.bottom - playerModel->GetWorldTransform()->translation_.y + kBlank) - (kHeight / 2.0f);
		info.move.y = std::max(0.0f, moveY);
		info.isTop = true;
	}
	else
	{
		info.isTop = false;
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info)
{

	if (info.move.y >= 0)
	{
		return;
	}


	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}


	//左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	//右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit) 
	{
		Vector3 offset = { 0.0f, kHeight / 2.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(playerModel->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.top - playerModel->GetWorldTransform()->translation_.y) + (kHeight / 2.0f) + kBlank;

		info.move.y = std::min(0.0f, moveY);
		info.isGround = true;
	}
	else 
	{
		info.isGround = false;
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info)
{
	if (info.move.x >= 0) 
	{
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	//左上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) 
	{
		hit = true;
	}
	//左下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) 
	{
		hit = true;
	}

	if (hit) 
	{
		Vector3 offset = { kWidth / 2.0f, 0.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(playerModel->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.right - playerModel->GetWorldTransform()->translation_.x + kWidth / 2 + kBlank;
		info.move.x = std::min(0.0f, moveX);
		info.isWall = true;
	}


}

void Player::CollisionMapRight(CollisionMapInfo& info)
{
	if (info.move.x <= 0)
	{
		return;
	}

	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	//右上
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
	{
		hit = true;
	}
	//右下
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) 
	{
		hit = true;
	}

	if (hit)
	{
		Vector3 offset = { kWidth / 2.0f, 0.0f, 0.0f };
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(playerModel->GetWorldTransform()->translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.left - playerModel->GetWorldTransform()->translation_.x - kWidth / 2 - kBlank;
		info.move.x = std::max(0.0f, moveX);
		info.isWall = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner)
{

	Vector3 offsetTable[kNumCorner] =
	{
		{+kWidth / 2.0f,-kHeight / 2.0f,0},
		{-kWidth / 2.0f,-kHeight / 2.0f,0},
		{+kWidth / 2.0f,+kHeight / 2.0f,0},
		{-kWidth / 2.0f,+kHeight / 2.0f,0},
	};

	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
}

void Player::CollisionMove(const CollisionMapInfo& info)
{
	playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, info.move);
}


void Player::HitTop(const CollisionMapInfo& info)
{
	if (info.isTop)
	{
		//ImGui::Text("hit");
		velocity_.y = 0;
	}
}
void Player::CollisionWall(const CollisionMapInfo& info)
{
	if (info.isWall)
	{
		velocity_.x *= (1.0f - kAttenuationWall);
		//velocity_.x = 0;
	}
}


void Player::SwitchGround(const CollisionMapInfo& info)
{

	if (onGround_)
	{
		//接地状態の処理
		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}
		else
		{
			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;

			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
			}

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				hit = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock)
			{
				hit = true;
			}

			//落下開始
			if (!hit)
			{
				//空中状態に切り替える
				onGround_ = false;
			}
		}
	}
	else
	{
		//空中状態の処理
		if (info.isGround)
		{
			//着地状態に切り替える
			onGround_ = true;
			//着地時にX速度を減衰
			velocity_.x *= (1.0f- kAttenuationLanding);
			//y座標をゼロにする
			velocity_.y = 0.0f;
		}
	}

}


