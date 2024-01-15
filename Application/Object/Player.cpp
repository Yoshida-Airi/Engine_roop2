#include"Player.h"

void Player::Initialize(ModelData playerData)
{
	input_ = Input::GetInstance();
	player = player->Create(playerData);

}

void Player::Update()
{
	player->Update();

	//移動
	Move();

	//旋回
	Rotate();


#ifdef _DEBUG
	Debug();
#endif // _DEBUG

	

}

void Player::Draw(ICamera* camera)
{
	player->Draw(camera);
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
	const float kMoveLimitX = 2.5f;
	const float kMoveLimitY = 0.8f;

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

void Player::Debug()
{
	ImGui::Begin("player");

	float translate[3] = { player->worldTransform_.translation_.x,player->worldTransform_.translation_.y,player->worldTransform_.translation_.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);

	player->worldTransform_.translation_ = { translate[0],translate[1],translate[2] };

	player->worldTransform_.UpdateWorldMatrix();

	ImGui::End();
}
