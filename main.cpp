#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"
#include"ImGuiManager.h"
#include"TextureManager.h"

#include"Triangle.h"
#include"Camera.h"
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


	TextureManager* texture = new TextureManager;
	texture->Initialize();


	Camera* camera = new Camera;
	camera->Initialize();

	Input* input = Input::GetInstance();
	input->Initialize();


#ifdef _DEBUG

	ImGuiManager* imgui = ImGuiManager::GetInstance();
	imgui->Initialize();
#endif // _DEBUG


	
	Triangle* triangle = new Triangle;
	triangle->Initialize();
	Triangle* triangle2 = new Triangle;
	triangle2->Initialize();
	

	
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		
		dxCommon->PreDraw();
		input->Update();
#ifdef _DEBUG
		imgui->Begin();
#endif // _DEBUG

#ifdef _DEBUG
		camera->cameraDebug();
#endif // _DEBUG


		triangle->Update();
		triangle->worldTransform.rotation_.y += 0.03f;

		triangle2->Update();
		triangle2->worldTransform.scale_.y = 0.5f;
		triangle2->worldTransform.rotation_.y += 0.02f;


		triangle->SetTextureSrvHandleGPU(texture->GetTextureSrvHandleGPU());
		triangle2->SetTextureSrvHandleGPU(texture->GetTextureSrvHandleGPU());

		triangle->Draw(camera);
		triangle2->Draw(camera);
	
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


	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;


	delete texture;
	texture = nullptr;

	delete camera;



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