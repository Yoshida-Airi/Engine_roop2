#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"

//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//WindowAPIの初期化
	WinApp* winApp = winApp->GetInstance();
	winApp->Initialize();
	
	//DirectXの初期化
	DirectXCommon* dxCommon = new DirectXCommon;
	dxCommon->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/



	}

	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;

	return 0;
}