#pragma once
#include"Logger.h"
#include"WinApp.h"
#include"DebugHelper.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<chrono>
#include<dxcapi.h>

#include<wrl.h>

#pragma comment(lib,"dxcompiler.lib")

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

	//シングルトン
	static DirectXCommon* GetInstance();

	/// <summary>
	/// バッファ生成
	/// </summary>
	/// <param name="sizeInBytes">サイズの指定</param>
	/// <returns>バッファ</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateBufferResource(size_t sizeInBytes);

	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap>CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	/*=======　　　ゲッター	=======*/
	ID3D12Device* GetDevice()const { return device.Get(); };
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); };
	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> GetSRVDescriptorHeap()const { return srvDescriptorHeap.Get(); };
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc()const { return swapChainDesc; };
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc()const { return rtvDesc; };

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
	/// ビューポートの生成
	/// </summary>
	void SetupViewport();

	/// <summary>
	/// シザー矩形の生成
	/// </summary>
	void SetupScissor();

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXCCompiler();

	/// <summary>
	/// PSOの設定
	/// </summary>
	void SetupPSO();

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void SetupRootSignature();

	/// <summary>
	/// インプットレイアウトの生成
	/// </summary>
	void SetupInputLayout();

	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	void SetupBlendState();

	/// <summary>
	/// ラスタライザ－ステートの生成
	/// </summary>
	void SetupRasterrizerState();

	/// <summary>
	/// シェーダーのコンパイル
	/// </summary>
	void SetupShader();

	/// <summary>
	/// コンパイルシェーダー
	/// </summary>
	/// <param name="filePath">シェーダーフィいる名</param>
	/// <param name="profile">プロファイル</param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="incledeHandler"></param>
	/// <returns></returns>
	IDxcBlob* CompileShader
	(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

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
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;		//RTV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvDescriptorHeap = nullptr;		//SRV用のディスクリプタヒープ
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	Microsoft::WRL::ComPtr < ID3D12Resource> swapChainResources[2] = { nullptr };	//スワップチェーンリソース
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_RESOURCE_BARRIER barrier{};	//トランスフォームバリア
	Microsoft::WRL::ComPtr< ID3D12Fence> fence = nullptr;	//フェンス
	uint64_t fenceValue = 0;	//フェンスの値
	HANDLE fenceEvent = nullptr;

	//記録時間(FPS固定)
	std::chrono::steady_clock::time_point reference_;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};	//DepthStensilStateの設定

	Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;

	Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState = nullptr;

	D3D12_VIEWPORT viewport{};	//ビューポート
	D3D12_RECT scissorRect{};	//シザー矩形

	//静的メンバ変数の宣言と初期化
	static DirectXCommon* instance;

};

