#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"

#include"Triangle.h"

//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region 基盤システムの初期化
	//WindowAPIの初期化
	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();
	

	//DirectXの初期化
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();
#pragma endregion

#pragma region シーンの初期化


#pragma endregion シーンの初期化

#pragma region 基盤システムの更新
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		dxCommon->PreDraw();
		dxCommon->PostDraw();
#pragma endregion 

#pragma region シーンの更新

#pragma endregion 

	}

#pragma region シーンの終了

#pragma endregion

#pragma region 基盤システムの終了

	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;

#pragma endregion 



	DebugHelper::ReportLiveObjects();

	return 0;
}