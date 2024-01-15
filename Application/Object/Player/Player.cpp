#include"Player.h"

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}
}

void Player::Initialize(const ModelData playerData, const ModelData bulletData)
{
	playerData_ = playerData;
	bulletData_ = bulletData;

	input_ = Input::GetInstance();
	playerModel_ = Model::Create(playerData_);

}

void Player::Update()
{
	playerModel_->Update();

	//旋回
	Rotate();
	//移動
	Move();
	//攻撃
	Attack();

	//弾の更新
	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Update();
	}

#ifdef _DEBUG
	Debug();
#endif // _DEBUG

	

}

void Player::Draw(ICamera* camera)
{
	playerModel_->Draw(camera);

	//弾の描画
	for (PlayerBullet*bullet : bullets_)
	{
		bullet->Draw(camera);
	}

}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = playerModel_->worldTransform_.matWorld_.m[3][0];
	worldpos.y = playerModel_->worldTransform_.matWorld_.m[3][1];
	worldpos.z = playerModel_->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//左右
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}

	//上下
	if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	}

	SumVector3(playerModel_->worldTransform_.translation_, move);

	//移動限界座標
	const float kMoveLimitX = 17.5f;
	const float kMoveLimitY = 9.0f;

	//範囲を超えない処理
	playerModel_->worldTransform_.translation_.x = max(playerModel_->worldTransform_.translation_.x, -kMoveLimitX);
	playerModel_->worldTransform_.translation_.x = min(playerModel_->worldTransform_.translation_.x, +kMoveLimitX);
	playerModel_->worldTransform_.translation_.y = max(playerModel_->worldTransform_.translation_.y, -kMoveLimitY);
	playerModel_->worldTransform_.translation_.y = min(playerModel_->worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	const float kRotSpeed = 0.2f;

	if (input_->PushKey(DIK_A))
	{
		playerModel_->worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		playerModel_->worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack()
{
	//ですフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});

	if (input_->TriggerKey(DIK_SPACE))
	{
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転
		velocity = TransformNormal(velocity, playerModel_->worldTransform_.matWorld_);

		//球を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletData_, GetWorldPosition(), velocity);

		//弾の登録
		bullets_.push_back(newBullet);

	}
}

void Player::Debug()
{
	ImGui::Begin("player");

	float translate[3] = { playerModel_->worldTransform_.translation_.x,playerModel_->worldTransform_.translation_.y,playerModel_->worldTransform_.translation_.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);

	playerModel_->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	playerModel_->worldTransform_.UpdateWorldMatrix();

	ImGui::End();
}
