#include "Framework.h"

Framework::~Framework()
{
	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;


	delete texture;
	texture = nullptr;


#ifdef _DEBUG
	DebugHelper::ReportLiveObjects();
#endif // _DEBUG



}

void Framework::Initialize()
{
	//WindowAPIの初期化
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	//DirectXの初期化
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();


	texture = TextureManager::GetInstance();
	texture->Initialize();

}

void Framework::Update()
{


}

void Framework::Draw()
{
	dxCommon->PreDraw();
	
}

void Framework::PostDraw()
{
	dxCommon->PostDraw();
}

void Framework::Run()
{
	Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage() == 0)
	{
		Update();
		Draw();
	}
}
