#include "Player.h"
#include"Object/CollisionConfig.h"


void Player::Initialize(const std::vector<Model*>& models)
{
	grobalVariables = GlobalVariables::GetInstance();
	groupName = "Player";

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeDef::kPlayer));
	Collider::SetColliderTypeID(static_cast<uint32_t>(ColliderType::SPHERE));

	GameObject::Initialize(models);

	playerModel = (models[0]);

	//モデルの初期設定
	playerModel->GetWorldTransform()->rotation_.y = std::numbers::pi_v<float> / 2.0f;
	playerModel->GetWorldTransform()->translation_.y += 5.0f;


	grobalVariables->AddItem(groupName, "Acceleration", kAcceleration);
	grobalVariables->AddItem(groupName, "Attenuation", kAttenuation);
	grobalVariables->AddItem(groupName, "LimitRunSpeed", kLimitRunSpeed);
	grobalVariables->AddItem(groupName, "GravityAcceleration", kGravityAcceleration);
	grobalVariables->AddItem(groupName, "LimitFallSpead", kLimitFallSpead);
	grobalVariables->AddItem(groupName, "JumpAcceleration", kJumpAcceleration);


	
}

void Player::Update()
{
	GameObject::Update();

	ApplyGlobalVariables();

	playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, velocity_);

	//移動処理
	Move();
	//旋回処理
	Turn();
	//ジャンプ処理
	Jump();

	playerModel->ModelDebug("player");

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
		playerModel->SetisInvisible(true);
	}
	else
	{
		playerModel->SetisInvisible(false);
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

			velocity_ = Add(velocity_, acceleration);

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
	if (onGround_)
	{
		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			//ジャンプ初速
			velocity_ = Add(velocity_, Vector3(0, kJumpAcceleration, 0));
		}

		if (velocity_.y > 0.0f)
		{
			onGround_ = false;
		}

	}
	else//空中
	{
		//落下速度
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpead);

		landing = false;

		//地面の当たり判定
		if (velocity_.y < 0)
		{
			//y座標が地面以下になったら着地
			if (playerModel->GetWorldTransform()->translation_.y <= 1.0f)
			{
				landing = true;
			}
		}

		//着地
		if (landing)
		{
			playerModel->GetWorldTransform()->translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
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
	



}
