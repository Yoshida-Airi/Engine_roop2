#include "Player.h"

void Player::Initialize()
{
	playerModel.reset(Model::Create("Resources/Object/Player/player.obj"));

	//モデルの初期設定
	playerModel->GetWorldTransform()->rotation_.y = std::numbers::pi_v<float> / 2.0f;
	playerModel->GetWorldTransform()->translation_.y += 1.0f;
}

void Player::Update()
{
	playerModel->Update();
	

	playerModel->GetWorldTransform()->translation_ = Add(playerModel->GetWorldTransform()->translation_, velocity_);

	//移動処理
	Move();
	//旋回処理
	Turn();

	playerModel->ModelDebug("player");

}

void Player::Draw(Camera* camera)
{
	playerModel->Draw(camera);
}

void Player::Move()
{
	/* --プレイヤーの移動処理-- */

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
