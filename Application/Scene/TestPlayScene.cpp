#include "TestPlayScene.h"
#include"numbers"
#include"ModelLoader.h"

TestPlayScene::~TestPlayScene()
{
	delete camera;
}

void TestPlayScene::Initialize()
{
	camera = new Camera;
	camera->Initialize();

	walkAnimation = Animation::GetInstance()->LoadAnimationFile("Resources/SampleAssets/human/walk.gltf");

	model.reset(Model::Create("Resources/SampleAssets/human/walk.gltf"));
	model->GetWorldTransform()->translation_.y = -0.7f;
	//model->SetAnimation(walkAnimation);
	//model->UseAnimation(false);
	model2.reset(Model::Create("Resources/SampleAssets/cube.obj"));
	model2->GetWorldTransform()->translation_.y = -1.8f;
	model2->GetWorldTransform()->scale_ = { 6.2f,1.0f,4.2f };
	

	model3.reset(Model::Create("Resources/SampleAssets/human/sneakWalk.gltf"));
	//model3->SetAnimation(walkAnimation);
	model3->SetisInvisible(true);

	model4.reset(Model::Create("Resources/SampleAssets/simpleSkin.gltf"));
	model4->GetWorldTransform()->rotation_.y = 3.14f;
	model4->SetisInvisible(true);

	model->MoveAnimation(false);

}

void TestPlayScene::Update()
{
	
	Move();
	if (isMoving == true)
	{
		model->MoveAnimation(true);
	}
	else
	{
		model->MoveAnimation(false);
	}

	

	camera->CameraDebug();
	model->Update();
	model2->Update();
	model3->Update();
	model4->Update();

	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		model->MoveAnimation(false);
	}

	

	model->ModelDebug("model");
	model2->ModelDebug("cube");
	model3->ModelDebug("walk");
	model4->ModelDebug("simpleSkin");

	
}

void TestPlayScene::Draw()
{
	model->Draw(camera);
	model2->Draw(camera);
	model3->Draw(camera);
	model4->Draw(camera);
}

void TestPlayScene::Move()
{
	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	Vector3 move = { 0.0f,0.0f,0.0f };

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		const float threshold = 0.9f;
		
		move = { 0,0,0 };

		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0)
		{
			// 移動量
			move =
			{
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
				0.0f,
				0.0f,//(float)joyState.Gamepad.sThumbLY / SHRT_MAX
			};

			float inputMagnitude = Length(move);

			// スティックの入力が一定の閾値以上の場合のみ移動処理を実行
			if (inputMagnitude > threshold)
			{
				isMoving = true;

				// スティックの入力に応じて速度を調整する
				float adjustedSpeed = speed * inputMagnitude;

				// 最大速度を超えないようにする
				if (adjustedSpeed > 0.05f)
				{
					adjustedSpeed = 0.05f;
				}

				// 実際の移動量を計算
				move.x *= adjustedSpeed;
				move.z *= adjustedSpeed;

				
				// 目標角度の算出
				angle = std::atan2(move.x, 0.0f);

				model->GetWorldTransform()->rotation_.y = LerpShortAngle(model->GetWorldTransform()->rotation_.y, angle, 0.1f);
				model->GetWorldTransform()->translation_.x += move.x;
				model->GetWorldTransform()->translation_.z += move.z;

			
			}
			
		}
		else
		{
			isMoving = false;
		}
	}


}

float TestPlayScene::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}


// 最短角度補間
float TestPlayScene::LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < 2 * (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= 2 * std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= (float)std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a, diff, t);
}