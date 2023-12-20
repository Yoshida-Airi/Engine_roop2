#include "Framework.h"

Framework::~Framework()
{

	delete texture;
	texture = nullptr;

	delete input;
	input = nullptr;


	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;


	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

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

	input = Input::GetInstance();
	input->Initialize();

}

void Framework::Update()
{


	input->Update();
	scene_->Update();

}

void Framework::Draw()
{
	dxCommon->PreDraw();
	scene_->Draw();
	dxCommon->PostDraw();
}

void Framework::Run()
{
	//初期化
	Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (winApp->ProcessMessage()==0)
	{
		//更新
		Update();
		//描画
		Draw();

	}
}
