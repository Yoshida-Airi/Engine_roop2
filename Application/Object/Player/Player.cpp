#include "Player.h"

Player::~Player()
{
	delete player;
	delete reticle3DModel;

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	for (int i = 0; i < 3; ++i)
	{
		delete Heart[i];

	}
}

void Player::Initialize(ICamera* camera, ICamera* UICamera, ModelData playerData, ModelData bullet)
{
	camera_ = camera;
	UICamera_ = UICamera;
	input = Input::GetInstance();
	texture = TextureManager::GetInstance();
	object = ModelLoader::GetInstance();

	playerData_ = playerData;
	bulletData_ = bullet;
	reticleData_= object->LoadObjFile("Resources", "cube.obj");

	player = new Model();
	player->Initialize(playerData_);
	player->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };

	reticle3DModel = new Model();
	reticle3DModel->Initialize(reticleData_);
	reticle3DModel->worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	

	for (int i = 0; i < 3; ++i)
	{
		Heart[i] = new Sprite();
		HeartData = texture->LoadTexture("Resources/Heart.png");
		Heart[i]->Initialize(HeartData);


	}
	
	Heart[1]->worldTransform.translation_ = { 40.0f };
	Heart[2]->worldTransform.translation_ = { 80.0f };


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
	reticle3DModel->Update();

	for (int i = 0; i < 3; ++i)
	{
		Heart[i]->Update();
	}

	SetReticle();


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
	reticle3DModel->Draw(camera_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::DrawUI()
{
	if (HP == 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			Heart[i]->Draw(UICamera_);
		}
	}

	if (HP == 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			Heart[i]->Draw(UICamera_);
		}
	}

	if (HP == 1)
	{
		for (int i = 0; i < 1; ++i)
		{
			Heart[i]->Draw(UICamera_);
		}
	}
}

Vector3 Player::GetReticleWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = reticle3DModel->worldTransform_.matWorld_.m[3][0];
	worldpos.y = reticle3DModel->worldTransform_.matWorld_.m[3][1];
	worldpos.z = reticle3DModel->worldTransform_.matWorld_.m[3][2];

	return worldpos;
}

void Player::Move()
{


	//移動
	Vector3 move = { 0,0,0 };
	const float kCharacterSpeed = 0.02f;

	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
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

	}

	
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
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
		Vector3 velocity;

		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(reticle3DModel->worldTransform_.translation_, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		////速度ベクトルを自機の向きに合わせて回転
		//velocity = TransformNormal(velocity, player->worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(camera_, bulletData_, GetWorldPosition(),velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			//弾の更新
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			//速度ベクトルを自機の向きに合わせて回転
			velocity = TransformNormal(velocity, player->worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(camera_, bulletData_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	}
}

void Player::SetReticle()
{
	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 10.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset,player->worldTransform_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	reticle3DModel->worldTransform_.translation_ = Add(GetWorldPosition(), offset);
	//ワールド行列の更新
	reticle3DModel->worldTransform_.UpdateWorldMatrix();

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		reticle3DModel->worldTransform_.translation_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		reticle3DModel->worldTransform_.translation_.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

	}


	if (HP == 3)
	{

	}

	////3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	//Vector3 positionReticle = GetReticleWorldPosition();
	////ビューポート行列
	//Matrix4x4 matViewport =
	//	MakeViewportMatrix(0, 0, WinApp::kCilentWidth, WinApp::kCilentHeight, 0, 1);
	////ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4x4 matViewProjectionViewport =
	//	Multiply(UICamera_->matView, Multiply(UICamera_->matProjection, matViewport));
	////ワールド→スクリーン座標変換(ここまで3Dから2Dになる)
	//positionReticle = Transform(positionReticle, matViewProjectionViewport);
	////スプライトのレティクルに座標設定
	//sprite2DReticle_->worldTransform.translation_.x = positionReticle.x;
	//sprite2DReticle_->worldTransform.translation_.y = positionReticle.y;



	//POINT mousePosition;
	//GetCursorPos(&mousePosition);

	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	//sprite2DReticle_->worldTransform.translation_.x = float(mousePosition.x);
	//sprite2DReticle_->worldTransform.translation_.y = float(mousePosition.y);

	//// ビューポート行列
	//Matrix4x4 matViewport =
	//	MakeViewportMatrix(0, 0, WinApp::kCilentWidth, WinApp::kCilentHeight, 0, 1);

	//// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	//Matrix4x4 matVPV =
	//	Multiply(UICamera_->matView, Multiply(UICamera_->matProjection, matViewport));

	////合成行列の逆行列を計算する
	//Matrix4x4 matInverseVPV = Inverse(matVPV);


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

void Player::OnCollision()
{
	HP -= 1;
}
