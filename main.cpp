#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"ImGuiManager.h"
#include"TextureManager.h"

#include"Triangle.h"
#include"Sprite.h"
#include"Camera.h"
#include"UICamera.h"
#include"Input.h"


//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	//WindowAPIの初期化
	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectXの初期化
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();


	TextureManager* texture = TextureManager::GetInstance();
	texture->Initialize();

	uint32_t uvTexture = texture->LoadTexture("Resources/uvChecker.png");
	uint32_t monsterBall = texture->LoadTexture("Resources/monsterBall.png");
	uint32_t Doll = texture->LoadTexture("Resources/Doll.png");

	Camera* camera = new Camera;
	camera->Initialize();

	UICamera* uiCamera = new UICamera;
	uiCamera->Initialize();

	Input* input = Input::GetInstance();
	input->Initialize();


#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	imgui->Initialize();
#endif // _DEBUG



	Triangle* triangle = new Triangle;
	triangle->Initialize(uvTexture);
	Triangle* triangle2 = new Triangle;
	triangle2->Initialize(monsterBall);

	Sprite* sprite = new Sprite;
	sprite->Initialize(Doll);
	sprite->SetSize({ 64.0f, 64.0f });
	sprite->SetTextureLeftTop({ 64,0 });
	sprite->SetTextureSize({ 64.0f, 64.0f });
	

	Sprite* sprite2 = new Sprite;
	sprite2->Initialize(uvTexture);
	sprite2->SetSize({ 640.0f, 360.0f });

	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		
		dxCommon->PreDraw();
		input->Update();

#ifdef _DEBUG
		imgui->Begin();
		camera->cameraDebug();

#endif // _DEBUG


		triangle->Update();
		triangle->worldTransform.rotation_.y += 0.03f;

		triangle2->Update();
		triangle2->worldTransform.scale_.y = 0.5f;
		triangle2->worldTransform.rotation_.y += 0.02f;

		sprite->worldTransform.translation_.x = 700.0f;

		sprite->Update();
		sprite2->Update();

		triangle->Draw(camera);
		triangle2->Draw(camera);
	
		sprite->Draw(uiCamera);
		sprite2->Draw(uiCamera);
		

		input->TriggerKey(DIK_0);


#ifdef _DEBUG
		ImGui::ShowDemoWindow();
		imgui->End();
		imgui->Draw();
#endif // _DEBUG

	
		dxCommon->PostDraw();
	
	}

	delete triangle;
	delete triangle2;
	delete sprite;
	delete sprite2;

	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;


	delete texture;
	texture = nullptr;

	delete camera;
	delete uiCamera;


	delete input;
	input = nullptr;
	

#ifdef _DEBUG
	delete imgui;
#endif // _DEBUG



#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG



	return 0;
}