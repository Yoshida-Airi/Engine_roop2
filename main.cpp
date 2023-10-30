#include<Windows.h>
#include"WinApp.h"

//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = new WinApp;
	winApp->Initialize();
	
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/



	}

	/*WindowsAPIの解放*/
	delete winApp;
	winApp = nullptr;


	return 0;
}