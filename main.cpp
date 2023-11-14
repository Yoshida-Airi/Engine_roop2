#include<Windows.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"DebugHelper.h"

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

	Triangle* triangle = new Triangle;
	triangle->Initialize();
	Triangle* triangle2 = new Triangle;
	triangle2->Initialize();
	TriangleData triangleData;
	triangleData.vertex[0] = { 0.2f,-0.2f ,0.0f,2.0f };
	triangleData.vertex[1] = { 0.4f,0.2f ,0.0f,2.0f };
	triangleData.vertex[2] = { 0.6f,-0.2f ,0.0f,2.0f };
	triangleData.color = { 0.0f, 1.0f, 0.0f, 1.0f };

	triangle2->SetVertexData(triangleData.vertex);
	triangle2->SetMaterialData(triangleData.color);

	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		/*--- ゲームループ  ---*/
		dxCommon->PreDraw();

		triangle->Draw();
		triangle2->Draw();

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



#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG



	return 0;
}