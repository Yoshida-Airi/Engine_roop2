#include "Framework.h"

Framework::~Framework()
{

	delete texture;
	texture = nullptr;

	delete audio;

	delete input;
	input = nullptr;


	//DirectXの解放
	delete dxCommon;
	dxCommon = nullptr;

	delete psoManager;

	//WindowAPIの解放
	delete winApp;
	winApp = nullptr;

	

	delete imgui;


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

	psoManager = GraphicsPipelineManager::GetInstance();
	psoManager->Initialize();

	texture = TextureManager::GetInstance();
	texture->Initialize();

	input = Input::GetInstance();
	input->Initialize();

	audio = Audio::GetInstance();
	audio->Initialize();

	imgui = ImGuiManager::GetInstance();
	imgui->Initialize();



	sceneManager_ = SceneManager::GetInstance();
	delete sceneFactory_;
}

void Framework::Update()
{

	imgui->Begin();



	input->Update();
	sceneManager_->Update();

}

void Framework::Draw()
{
	dxCommon->PreDraw();
	sceneManager_->Draw();

	ImGui::ShowDemoWindow();
	imgui->End();
	imgui->Draw();

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
