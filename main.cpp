#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"ImGuiManager.h"
#include"TextureManager.h"

#include"Triangle.h"
#include"Camera.h"

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

	texture->CreateTexture();

	Camera* camera = new Camera;
	camera->Initialize();

#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	imgui->Initialize();
#endif // _DEBUG

	
	Triangle* triangle = new Triangle;
	triangle->Initialize();

	

	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		
		dxCommon->PreDraw();
#ifdef _DEBUG
		imgui->Begin();
#endif // _DEBUG

	
		triangle->Update();
		triangle->worldTransform.rotation_.y += 0.03f;

		triangle->Draw(camera);
		
		bool label = false;

#ifdef _DEBUG
		ImGui::ShowDemoWindow();
		imgui->End();
		imgui->Draw();
#endif // _DEBUG

	
		dxCommon->PostDraw();
	
	}

	delete triangle;


	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;

	delete camera;

#ifdef _DEBUG
	delete imgui;
#endif // _DEBUG



#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG



	return 0;
}