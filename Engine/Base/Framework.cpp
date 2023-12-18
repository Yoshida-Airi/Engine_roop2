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

	delete sceneManager_;

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

	sceneManager_ = new SceneManager();

}

void Framework::Update()
{
	sceneManager_->Update();
}

void Framework::Draw()
{
	dxCommon->PreDraw();
	sceneManager_->Draw();
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
