#include "TitleScene.h"
#include"SceneManager.h"

TitleScene::~TitleScene()
{
	delete camera;
	delete uiCamera;
}

void TitleScene::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	camera = new Camera;
	camera->Initialize();

	uiCamera = new UICamera;
	uiCamera->Initialize();

	titleTex = TextureManager::GetInstance()->LoadTexture("Resources/title.png");
	ABottonTex = TextureManager::GetInstance()->LoadTexture("Resources/ABotton.png");

	fence_ = Model::Create("Resources", "fence.obj");
	fence_->SetisInvisible(true);

	cube_ = Model::Create("Resources", "enemy.obj");

	

	titleSprite.reset(Sprite::Create(titleTex));
	ABottonSprite.reset(Sprite::Create(ABottonTex));
	ABottonSprite->SetPosition({ 610.0f,500.0f });

	InitializeFloatingGimmick();
	
}

void TitleScene::Update()
{
	camera->CameraDebug();
	camera->UpdateMatrix();

	uiCamera->UpdateMatrix();

	input->TriggerKey(DIK_0);


	//シーン遷移
	if (input->TriggerKey(DIK_RETURN))
	{
		sceneManager_->ChangeScene("GAMEPLAY");
	}

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
		}
		
	}

	UpAndDownMotion(1.5f);


	fence_->Update();
	cube_->Update();

	titleSprite->Update();
	ABottonSprite->Update();

	cube_->ModelDebug("cube");
	fence_->ModelDebug("fence");

	camera->matView = {
		1.0f,1.0f,1.0f,1.0f,
		3.0f,0.0f,1.0f,2.0f,
	};

	bool drawEnabled = true;
	flashTime++;

	// 描画を点滅させるための処理
	if (drawEnabled && flashTime >= 40) {
		ABottonSprite->SetisInvisible(false);
		
	}

	if (flashTime >= 80)
	{

		// フラグをトグル
		ABottonSprite->SetisInvisible(true);
		drawEnabled = !drawEnabled;
		flashTime = 0;
	}

	
}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);


	titleSprite->Draw(uiCamera);
	ABottonSprite->Draw(uiCamera);
}


void TitleScene::InitializeFloatingGimmick() {

	//浮遊ギミックの媒介変数
	UpdownParameter_ = 0.0f;
	swingParameter_ = 0.0f;
	//浮遊移動のサイクル<frame>
	uint16_t cycle_ = 60;
	//浮遊の振動<m>
	amplitude_ = 1.0f;
}

void TitleScene::UpAndDownMotion(float time)
{
	//1フレームでのパラメータ加算値
	const float step = time * 3.14f / cycle_;
	//パラメータを１ステップ分加算
	UpdownParameter_ += step;
	//2πを超えたら０に戻す
	UpdownParameter_ = std::fmod(UpdownParameter_, 2.0f * 3.14f);
	//浮遊を座標に反映
	cube_->worldTransform_->translation_.y = (std::sin(UpdownParameter_) * amplitude_);
}

