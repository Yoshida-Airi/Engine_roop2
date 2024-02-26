#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SrvManager.h"

#include"imgui_impl_dx12.h"
#include"imgui_impl_win32.h"



class ImGuiManager
{
public:

	static ImGuiManager* GetInstance();

	~ImGuiManager();

	void Initialize();

	/// <summary>
	/// フレームのはじめ
	/// </summary>
	void Begin();

	/// <summary>
	/// 内部コマンドの生成
	/// </summary>
	void End();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	WinApp* winApp_;
	DirectXCommon* dxCommon_;
	SrvManager* srvManager_;

	static ImGuiManager* instance;

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;

};

