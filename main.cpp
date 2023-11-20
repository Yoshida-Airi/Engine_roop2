#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"ImGuiManager.h"
#include"TextureManager.h"

#include"Triangle.h"

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

#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	imgui->Initialize();
#endif // _DEBUG

	TriangleData triangleData;
	triangleData.vertex[0] = { -0.5f,-0.5f,0.0f,1.0f };
	triangleData.vertex[1] = { 0.0f,0.5f,0.0f,1.0f };
	triangleData.vertex[2] = { 0.5f,-0.5f,0.0f,1.0f };

	Triangle* triangle = new Triangle;
	triangle->Initialize(triangleData);

	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		
		dxCommon->PreDraw();
#ifdef _DEBUG
		imgui->Begin();
#endif // _DEBUG

	
		triangle->Update();
		
		triangle->Draw();
		
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

#ifdef _DEBUG
	delete imgui;
#endif // _DEBUG



#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG



	return 0;
}