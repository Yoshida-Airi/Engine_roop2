#include "Player.h"

Player::~Player()
{
	delete player;

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(ICamera* camera, ModelData playerData, ModelData bullet)
{
	camera_ = camera;
	input = Input::GetInstance();

	playerData_ = playerData;
	bulletData_ = bullet;
	
	player = new Model();
	player->Initialize(playerData_);
	player->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };


}

void Player::Update()
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet)
	{
		if (bullet->isDead())
		{
			delete bullet;
			return true;
		}
		return false;
	});


	//回転
	Rotate();

	//移動
	Move();

	//攻撃
	Attack();

	player->Update();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

#ifdef _DEBUG
	ImGui::Begin("player");

	float translate[3] = { player->worldTransform_.translation_.x,player->worldTransform_.translation_.y,player->worldTransform_.translation_.z };
	ImGui::SliderFloat3("transform", translate, -10, 4);

	player->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	ImGui::End();

#endif // _DEBUG

}

void Player::Draw()
{
	player->Draw(camera_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::Move()
{

	//移動
	Vector3 move = { 0,0,0 };
	const float kCharacterSpeed = 0.02f;

	if (input->PushKey(DIK_LEFT))
	{
		move.x -= kCharacterSpeed;
	}
	else if (input->PushKey(DIK_RIGHT))
	{
		move.x += kCharacterSpeed;
	}

	if (input->PushKey(DIK_UP))
	{
		move.y += kCharacterSpeed;
	}
	else if (input->PushKey(DIK_DOWN))
	{
		move.y -= kCharacterSpeed;
	}

	player->worldTransform_.translation_.x += move.x;
	player->worldTransform_.translation_.y += move.y;


	//移動限界範囲
	const float kMoveLimitX = 3.5f;
	const float kMoveLimitY = 1.7f;

	player->worldTransform_.translation_.x = max(player->worldTransform_.translation_.x, -kMoveLimitX);
	player->worldTransform_.translation_.x = min(player->worldTransform_.translation_.x, +kMoveLimitX);
	player->worldTransform_.translation_.y = max(player->worldTransform_.translation_.y, -kMoveLimitY);
	player->worldTransform_.translation_.y = min(player->worldTransform_.translation_.y, +kMoveLimitY);


}

void Player::Rotate()
{
	//回転
	const float kRotSpeed = 0.02f;
	if (input->PushKey(DIK_A))
	{
		player->worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input->PushKey(DIK_D))
	{
		player->worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//弾の更新
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転
		velocity = TransformNormal(velocity, player->worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(camera_, bulletData_, GetWorldPosition(),velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = player->worldTransform_.matWorld_.m[3][0];
	worldpos.y = player->worldTransform_.matWorld_.m[3][1];
	worldpos.z = player->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}
