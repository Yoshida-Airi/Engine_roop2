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
	fadeTex= TextureManager::GetInstance()->LoadTexture("Resources/black.png");

	fence_ = Model::Create("Resources", "fence.obj");
	fence_->SetisInvisible(true);

	cube_ = Model::Create("Resources", "enemy.obj");

	

	titleSprite.reset(Sprite::Create(titleTex));
	ABottonSprite.reset(Sprite::Create(ABottonTex));
	ABottonSprite->SetPosition({ 610.0f,500.0f });
	fadeSprite.reset(Sprite::Create(fadeTex));
	fadeSprite->SetSize({ 1280,720 });
	fadeSprite->SetisInvisible(true);
	alpha = 0;
	fadeSprite->SetMaterialData({ 1.0f,1.0f,1.0f,alpha });


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
		
		// フェードイン開始
		StartFadeIn();
		
	}

	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			// フェードイン開始
			StartFadeIn();
		}
		
	}

	UpAndDownMotion(1.5f);


	fence_->Update();
	cube_->Update();

	titleSprite->Update();
	ABottonSprite->Update();
	fadeSprite->Update();

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

	// フェードイン中の処理
	if (isFadingIn)
	{
		UpdateFadeIn();
	}
	
}

void TitleScene::Draw()
{
	fence_->Draw(camera);
	cube_->Draw(camera);


	titleSprite->Draw(uiCamera);
	ABottonSprite->Draw(uiCamera);
	fadeSprite->Draw(uiCamera);
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

void TitleScene::StartFadeIn()
{
	isFadingIn = true;
	fadeSprite->SetisInvisible(false);
}

void TitleScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	fadeSprite->SetMaterialData({ 1.0f, 1.0f, 1.0f, alpha });

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		sceneManager_->ChangeScene("GAMEPLAY");
	}
}