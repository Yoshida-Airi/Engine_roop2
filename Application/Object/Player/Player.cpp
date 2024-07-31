#define NOMINMAX

#include "Player.h"
#include"Object/CollisionConfig.h"
#include"Object/Player/Weapon.h"
#include"Object/Ground/Ground.h"

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
	playerModel->GetWorldTransform()->translation_.y += 7.0f;


	grobalVariables->AddItem(groupName, "Acceleration", kAcceleration);
	grobalVariables->AddItem(groupName, "Attenuation", kAttenuation);
	grobalVariables->AddItem(groupName, "LimitRunSpeed", kLimitRunSpeed);
	grobalVariables->AddItem(groupName, "GravityAcceleration", kGravityAcceleration);
	grobalVariables->AddItem(groupName, "LimitFallSpead", kLimitFallSpead);
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
	playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, velocity_);

	collisionMapInfo.move = velocity_;
	//マップ衝突チェック
	CollisionMap(collisionMapInfo);

	CollisionMove(collisionMapInfo);
	HitTop(collisionMapInfo);
	CollisionWall(collisionMapInfo);
	SwitchGround(collisionMapInfo);
	

	ImGui::Text("x %d", collisionMapInfo.move.x);
	ImGui::Text("y %d", collisionMapInfo.move.y);
	ImGui::Text("z %d", collisionMapInfo.move.z);
	ImGui::Text("isTop %d", collisionMapInfo.isTop);
	ImGui::Text("isGround %d", collisionMapInfo.isGround);
	ImGui::Text("isWall %d", collisionMapInfo.isWall);
	ImGui::Text("grand %d", onGround_);
	//ImGui::Text("landing %d", landing);

	//ImGui::Text("%d", onGround_);
	//ImGui::Text("%d", landing);

	//float kGravityAcceleration_[1] = { kGravityAcceleration };
	//ImGui::DragFloat("kGravityAcceleration", kGravityAcceleration_, 0.1f);
	//kGravityAcceleration = kGravityAcceleration_[0];

	//float kLimitFallSpead_[1] = { kLimitFallSpead };
	//ImGui::DragFloat("kLimitFallSpead", kLimitFallSpead_, 0.1f);
	//kLimitFallSpead = kLimitFallSpead_[0];


	//float kJumpAcceleration_[1] = { kJumpAcceleration };
	//ImGui::DragFloat("kJumpAcceleration", kJumpAcceleration_, 0.1f);
	//kJumpAcceleration = kJumpAcceleration_[0];


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
	if (typeID == static_cast<uint32_t>(CollisionTypeDef::kMap))
	{
		//playerModel->SetisInvisible(true);
	}
	else
	{
		//playerModel->SetisInvisible(false);
	}
}

void Player::Move()
{
	/* --プレイヤーの移動処理-- */

	//接地状態
	if (onGround_)
	{
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))
		{
			//左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				if (velocity_.x < 0.0f)
				{
					//速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				//モデルの視線を動きに合わせる
				if (lrDirection != LRDirection::kRight)
				{
					lrDirection = LRDirection::kRight;
					turnFirstRotationY = playerModel->GetWorldTransform()->rotation_.y;
					turnTimer = kTimeTrun;
				}
			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				if (velocity_.x > 0.0f)
				{
					//速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				//モデルの視線を動きに合わせる
				if (lrDirection != LRDirection::kLeft)
				{
					lrDirection = LRDirection::kLeft;
					turnFirstRotationY = playerModel->GetWorldTransform()->rotation_.y;
					turnTimer = kTimeTrun;
				}
			}

			velocity_.x += acceleration.x;

			//速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		}
		else
		{
			//非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
	}
}

void Player::Turn()
{
	/* --プレイヤーの旋回処理-- */

	if (turnTimer > 0.0f)
	{
		turnTimer = 1.0f / 60.0f;

		float destinationRotationYTable[] =
		{
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float>*3.0f / 2.0f
		};
		//角度の取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection)];
		//角度を変更する
		playerModel->GetWorldTransform()->rotation_.y = LerpShortTranslate(playerModel->GetWorldTransform()->rotation_.y, destinationRotationY, turnTimer);
	}
}

void Player::Jump()
{
	//接地状態
	if (onGround_ )
	{
		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			if (!isJump) {
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
		velocity_.y = std::max(velocity_.y, -kLimitFallSpead);

		//landing = false;

		////着地
		//if (landing)
		//{
		//	playerModel->GetWorldTransform()->translation_.y = 1.0f;
		//	velocity_.x *= (1.0f - kAttenuation);
		//	velocity_.y = 0.0f;
		//	onGround_ = true;
		//}
	}

	
}

void Player::ApplyGlobalVariables()
{
	kAcceleration = grobalVariables->GetFloatValue(groupName, "Acceleration");
	kAttenuation = grobalVariables->GetFloatValue(groupName, "Attenuation");
	kLimitRunSpeed = grobalVariables->GetFloatValue(groupName, "LimitRunSpeed");
	kGravityAcceleration = grobalVariables->GetFloatValue(groupName, "GravityAcceleration");
	kLimitFallSpead = grobalVariables->GetFloatValue(groupName, "LimitFallSpead");
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
	Jump();



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
	//CollisionMapLeft(info);
	//CollisionMapRight(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info)
{
	//天井の当たり判定

	bool hit = false;


	//上昇ありかどうか
	if (info.move.y <= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner>positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	for (const auto& ground : ground_)
	{
		//左上点の判定
		if (IsCollision(positionsNew[kLeftTop], ground->GetAABB()))
		{
			hit = true;
		}


		//右上点の判定
		if (IsCollision(positionsNew[kRightTop], ground->GetAABB()))
		{
			hit = true;
		}



		if (hit)
		{
			Rect rect = GetRect(ground);
			float move = (rect.bottom - playerModel->GetWorldTransform()->translation_.y)-  (playerModel->GetWorldTransform()->scale_.y / 2.0f + kBlank);
			info.move.y = std::min(0.0f, move);
			info.isTop = true;
		}
		else
		{
			info.isTop = false;
		}
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info)
{
	bool hit = false;
	//下降ありかどうか
	if (info.move.y >= 0)
	{
		return;
	}

	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner>positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	for (const auto& ground : ground_)
	{
		//左下点の判定
		if (IsCollision(positionsNew[kLeftBottom], ground->GetAABB()))
		{
			hit = true;
		}
		//右下点の判定
		if (IsCollision(positionsNew[kRightBottom], ground->GetAABB()))
		{
			hit = true;
		}


		if (hit)
		{
			Rect rect = GetRect(ground);
			float move = (rect.top - playerModel->GetWorldTransform()->translation_.y) + (playerModel->GetWorldTransform()->scale_.y / 2.0f + kBlank);
			info.move.y = std::min(0.0f, move);
			info.isGround = true;
		}
		else
		{
			info.isGround = false;
		}
	}
}

void Player::CollisionMapLeft(CollisionMapInfo& info)
{
	bool hit = false;

	//右移動あり
	if (velocity_.x >= 0)
	{
		return;
	}


	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner>positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	for (const auto& ground : ground_)
	{
		//左上点の判定
		if (IsCollision(positionsNew[kLeftTop], ground->GetAABB()))
		{
			hit = true;
		}

		//左下点の判定
		if (IsCollision(positionsNew[kLeftBottom], ground->GetAABB()))
		{
			hit = true;
		}


		if (hit)
		{
			Rect rect = GetRect(ground);
			float move = (rect.left - playerModel->GetWorldTransform()->translation_.x) + (playerModel->GetWorldTransform()->scale_.x / 2.0f + kBlank);
			info.move.x = std::min(0.0f, move);
			info.isWall = true;
		}
		else
		{
			info.isWall = false;
		}
	}

}

void Player::CollisionMapRight(CollisionMapInfo& info)
{
	bool hit = false;

	//右移動あり
	if (velocity_.x <= 0)
	{
		return;
	}


	//移動後の4つの角の座標
	std::array<Vector3, kNumCorner>positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i)
	{
		positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
	}

	for (const auto& ground : ground_)
	{
		//右上点の判定
		if (IsCollision(positionsNew[kRightTop], ground->GetAABB()))
		{
			hit = true;
		}

		//右下点の判定
		if (IsCollision(positionsNew[kRightBottom], ground->GetAABB()))
		{
			hit = true;
		}

		if (hit)
		{
			Rect rect = GetRect(ground);
			float move = (rect.right - playerModel->GetWorldTransform()->translation_.x) - (playerModel->GetWorldTransform()->scale_.x / 2.0f + kBlank);
			info.move.x = std::max(0.0f, move);
			info.isWall = true;
			//return;
		}
		else
		{
			info.isWall = false;
		}
	}
	//info.isWall = false;
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

Player::Rect Player::GetRect(Ground* ground)
{
	//ブロックの中心座標を取得
	Vector3 center = ground->GetWorldPosition();

	// ブロックのスケールを取得


	// 矩形領域を計算
	Rect rect;
	rect.left = center.x - ground->GetRadius().x;
	rect.right = center.x + ground->GetRadius().x;
	rect.bottom = center.y - ground->GetRadius().y;
	rect.top = center.y + ground->GetRadius().y;

	return rect;

}

void Player::HitTop(const CollisionMapInfo& info)
{
	if (info.isTop)
	{
		ImGui::Text("hit");
		velocity_.y = 0;
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
			bool hit = false;
			//移動後の4つの角の座標
			std::array<Vector3, kNumCorner>positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i)
			{
				positionsNew[i] = CornerPosition(Add(playerModel->GetWorldTransform()->translation_, info.move), static_cast<Corner>(i));
			}

			for (const auto& ground : ground_)
			{
				//左下点の判定
				if (IsCollision(Add(positionsNew[kLeftBottom], Vector3(0, -0.01f, 0)), ground->GetAABB()))
				{
					hit = true;
				}
				//右下点の判定
				if (IsCollision(Add(positionsNew[kRightBottom], Vector3(0, -0.01f, 0)), ground->GetAABB()))
				{
					hit = true;
				}
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
			velocity_.x *= (1.0f, kAttenuationLanding);
			//y座標をゼロにする
			velocity_.y = 0.0f;
		}
	}


}

void Player::CollisionWall(const CollisionMapInfo& info)
{
	if (info.isWall)
	{
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}
