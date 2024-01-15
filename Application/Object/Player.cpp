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
	player = player->Create(playerData);

}

void Player::Update()
{
	player->Update();

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
	player->Draw(camera);

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
	worldpos.x = player->worldTransform_.matWorld_.m[3][0];
	worldpos.y = player->worldTransform_.matWorld_.m[3][1];
	worldpos.z = player->worldTransform_.matWorld_.m[3][2];

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

	SumVector3(player->worldTransform_.translation_, move);

	//移動限界座標
	const float kMoveLimitX = 17.5f;
	const float kMoveLimitY = 9.0f;

	//範囲を超えない処理
	player->worldTransform_.translation_.x = max(player->worldTransform_.translation_.x, -kMoveLimitX);
	player->worldTransform_.translation_.x = min(player->worldTransform_.translation_.x, +kMoveLimitX);
	player->worldTransform_.translation_.y = max(player->worldTransform_.translation_.y, -kMoveLimitY);
	player->worldTransform_.translation_.y = min(player->worldTransform_.translation_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	const float kRotSpeed = 0.2f;

	if (input_->PushKey(DIK_A))
	{
		player->worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		player->worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		

		//球を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(bulletData_, GetWorldPosition());

		//弾の登録
		bullets_.push_back(newBullet);

	}
}

void Player::Debug()
{
	ImGui::Begin("player");

	float translate[3] = { player->worldTransform_.translation_.x,player->worldTransform_.translation_.y,player->worldTransform_.translation_.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);

	player->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	player->worldTransform_.UpdateWorldMatrix();

	ImGui::End();
}
