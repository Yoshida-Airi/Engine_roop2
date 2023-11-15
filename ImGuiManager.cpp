//#include "ImGuiManager.h"
//
//ImGuiManager* ImGuiManager::GetInstance()
//{
//	if (instance == NULL)
//	{
//		instance = new ImGuiManager;
//	}
//
//	return instance;
//}
//
//ImGuiManager::~ImGuiManager()
//{
//	ImGui_ImplDX12_Shutdown();
//	ImGui_ImplWin32_Shutdown();
//	ImGui::DestroyContext();
//}
//
//void ImGuiManager::Initialize()
//{
//	winApp_ = WinApp::GetInstance();
//	dxCommon_ = DirectXCommon::GetInstance();
//
//	srvDescriptorHeap_ = dxCommon_->GetSRVDescriptorHeap();
//
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui::StyleColorsDark();
//	ImGui_ImplWin32_Init(winApp_->GetHwnd());
//	ImGui_ImplDX12_Init(dxCommon_->GetDevice(),
//		dxCommon_->GetSwapChainDesc().BufferCount,
//		dxCommon_->GetRtvDesc().Format,
//		srvDescriptorHeap_.Get(),
//		srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(),
//		srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart());
//}
//
//void ImGuiManager::Begin()
//{
//	ImGui_ImplDX12_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//
//	ImGui::ShowDemoWindow();
//}
//
//void ImGuiManager::End()
//{
//	ImGui::Render();
//	dxCommon_->GetCommandList()->SetDescriptorHeaps(1, srvDescriptorHeap_.GetAddressOf());
//}
//
//void ImGuiManager::Draw()
//{
//	//実際のcommandListのImGuiの描画コマンド
//	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon_->GetCommandList());
//}
//
//ImGuiManager* ImGuiManager::instance = NULL;