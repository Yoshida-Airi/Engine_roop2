#include"Player.h"
#include"CollisionConfig.h"

Player::~Player()
{
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}


}

void Player::Initialize(Vector3 pos, ICamera* camera)
{
	
	input_ = Input::GetInstance();
	playerModel_ = Model::Create("Resources", "cube.obj"); 
	camera_ = camera;

	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionAttributeEnemyBullet);

	playerModel_->worldTransform_->translation_ = pos;

	reticleModel= Model::Create("Resources", "cube.obj");
	sprite2DReticle_.reset(Sprite::Create(textureReticle));
	sprite2DReticle_->SetAnchorPoint({ 0.5f,0.5f });
	sprite2DReticle_->worldTransform.translation_ = { 640.0f,360.0f };

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
	//Set3DReticle(camera_);


	Set3DReticleMousePosition(camera_);

	sprite2DReticle_->Update();

};


void Player::Draw(ICamera* camera)
{
	playerModel_->Draw(camera);
	reticleModel->Draw(camera);

	//弾の描画
	for (PlayerBullet*bullet : bullets_)
	{
		bullet->Draw(camera);
	}

}

void Player::DrawUI(ICamera* camera)
{
	sprite2DReticle_->Draw(camera);
}

Vector3 Player::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = playerModel_->worldTransform_->matWorld_.m[3][0];
	worldpos.y = playerModel_->worldTransform_->matWorld_.m[3][1];
	worldpos.z = playerModel_->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

Vector3 Player::GetReticleWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldpos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldpos.x = reticleModel->worldTransform_->matWorld_.m[3][0];
	worldpos.y = reticleModel->worldTransform_->matWorld_.m[3][1];
	worldpos.z = reticleModel->worldTransform_->matWorld_.m[3][2];

	return worldpos;
}

void Player::OnCollision()
{
}

void Player::SetParent(const WorldTransform* transform)
{
	playerModel_->worldTransform_->parent_ = transform;
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

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	SumVector3(playerModel_->worldTransform_->translation_, move);

	//移動限界座標
	const float kMoveLimitX = 17.5f;
	const float kMoveLimitY = 9.0f;

	//範囲を超えない処理
	playerModel_->worldTransform_->translation_.x = max(playerModel_->worldTransform_->translation_.x, -kMoveLimitX);
	playerModel_->worldTransform_->translation_.x = min(playerModel_->worldTransform_->translation_.x, +kMoveLimitX);
	playerModel_->worldTransform_->translation_.y = max(playerModel_->worldTransform_->translation_.y, -kMoveLimitY);
	playerModel_->worldTransform_->translation_.y = min(playerModel_->worldTransform_->translation_.y, +kMoveLimitY);
}

void Player::Rotate()
{
	const float kRotSpeed = 0.2f;

	if (input_->PushKey(DIK_A))
	{
		playerModel_->worldTransform_->rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		playerModel_->worldTransform_->rotation_.y += kRotSpeed;
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

	XINPUT_STATE joyState;

	if (input_->TriggerKey(DIK_SPACE))
	{
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(reticleModel->worldTransform_->translation_, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		////速度ベクトルを自機の向きに合わせて回転
		//velocity = TransformNormal(velocity, playerModel_->worldTransform_->matWorld_);

		//球を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity);

		//弾の登録
		bullets_.push_back(newBullet);

	}


	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity;

			// 速度ベクトルを自機の向きに合わせて回転させる
			velocity = TransformNormal(velocity, playerModel_->worldTransform_->matWorld_);

			//自機から照準オブジェクトへのベクトル
			velocity = Subtract(GetReticleWorldPosition(), GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalize(velocity));



			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		}
	}
}

void Player::Debug()
{
	ImGui::Begin("player");

	float translate[3] = { playerModel_->worldTransform_->translation_.x,playerModel_->worldTransform_->translation_.y,playerModel_->worldTransform_->translation_.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);

	playerModel_->worldTransform_->translation_ = { translate[0],translate[1],translate[2] };

	playerModel_->worldTransform_->UpdateWorldMatrix();

	ImGui::End();
}

void Player::Set3DReticle(const ICamera* camera)
{
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 100.0f;
	//自機から3Dレティクルへのオフセット
	Vector3 offset = { 0,0,1.0f };
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset, playerModel_->worldTransform_->matWorld_);
	//ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	//3Dレティクルの座標を設定
	reticleModel->worldTransform_->translation_ = Add(GetWorldPosition(), offset);
	//ワールド行列の更新
	reticleModel->worldTransform_->UpdateWorldMatrix();

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = GetReticleWorldPosition();
	//ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kCilentWidth, WinApp::kCilentHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		Multiply(camera->matView, Multiply(camera->matProjection, matViewport));
	//ワールド→スクリーン座標変換(ここまで3Dから2Dになる)
	positionReticle = CoorTransform(positionReticle, matViewProjectionViewport);
	//スプライトのレティクルに座標設定
	sprite2DReticle_->worldTransform.translation_ = { positionReticle.x, positionReticle.y };


}


void Player::Set3DReticleMousePosition(const ICamera* camera) 
{
	
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kCilentWidth, WinApp::kCilentHeight, 0, 1);

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matVPV =
		Multiply(camera->matView, Multiply(camera->matProjection, matViewport));

	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear =
		Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
		Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	Vector2 spritePosirion = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		spritePosirion.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosirion.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;

		sprite2DReticle_->SetPosition(spritePosirion);

	}


	//スクリーン座標系からワールド座標系へ
	posNear = CoorTransform(posNear, matInverseVPV);
	posFar = CoorTransform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);

	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	reticleModel->worldTransform_->translation_ =
		Add(posNear, Multiply(kDistanceTestObject, mouseDirection));

	reticleModel->worldTransform_->UpdateWorldMatrix();

#ifdef _DEBUG
	ImGui::Begin("reticle");
	ImGui::Text(
		"2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+2f,%+2f,%+2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+2f,%+2f,%+2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
		"3DReticle:(%+2f,%+2f,%+2f)", reticleModel->worldTransform_->translation_.x,
		reticleModel->worldTransform_->translation_.y, reticleModel->worldTransform_->translation_.z);
	ImGui::End();
#endif // _DEBUG

	
}