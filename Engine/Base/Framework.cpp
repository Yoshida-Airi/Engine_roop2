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

	delete scene;

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

	scene = new Game;
	scene->Initialize();

}

void Framework::Update()
{
	scene->Update();
}

void Framework::Draw()
{
	dxCommon->PreDraw();
	scene->Draw();
	dxCommon->PostDraw();
}

void Framework::PostDraw()
{
	
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
