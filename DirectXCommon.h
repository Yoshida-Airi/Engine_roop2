#pragma once
#include"Logger.h"
#include"WinApp.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<dxgidebug.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

#include<wrl.h>

class DirectXCommon
{
public:
	~DirectXCommon();
	//初期化処理
	void Initialize();
	//更新処理
	void Update();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();

	/// <summary>
	/// 解放漏れがある場合止める
	/// </summary>
	void ReportLiveObjects();

private:

	/// <summary>
	/// デバイスの生成
	/// </summary>
	void SetupDevice();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void SetupCommand();

	/// <summary>
	///　スワップチェインの生成
	/// </summary>
	void SetupSwapChain();

	/// <summary>
	/// レンダーターゲットビューの生成
	/// </summary>
	void SetupRnderTargetView();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void SetupDepthBuffer();

	/// <summary>
	/// フェンスの生成
	/// </summary>
	void SetupFence();

	/// <summary>
	/// レンダーターゲットクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// デバッグレイヤー
	/// </summary>
	void SetupDebugLayer();

	/// <summary>
	/// エラーで止める、抑制
	/// </summary>
	void ConfigDebugMessageFilter();


private:
	
	WinApp* winApp_;

	HRESULT hr;
	Microsoft::WRL::ComPtr< IDXGIFactory7> dxgiFactory = nullptr;	//DXGIFactory
	Microsoft::WRL::ComPtr <IDXGIAdapter4> useAdapter = nullptr;	//使用するアダプタ用の変数。
	Microsoft::WRL::ComPtr< ID3D12Device> device = nullptr;			//デバイス
	Microsoft::WRL::ComPtr< ID3D12CommandQueue> commandQueue = nullptr;	//コマンドキュー
	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;	//コマンドアロケータ
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;	//コマンドリスト
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain = nullptr;	//スワップチェーン
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;		//RTV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources[2] = { nullptr };	//スワップチェーンリソース
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_RESOURCE_BARRIER barrier{};	//トランスフォームバリア
	Microsoft::WRL::ComPtr< ID3D12Fence> fence = nullptr;	//フェンス
	uint64_t fenceValue = 0;	//フェンスの値
	HANDLE fenceEvent = nullptr;
};

