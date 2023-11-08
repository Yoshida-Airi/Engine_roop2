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


	/*=======　　　ゲッター	=======*/
	ID3D12Device* GetDevice()const { return device.Get(); };
	ID3D12GraphicsCommandList* GetCommandList()const { return commandList.Get(); };

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
	void IntializeDXC();

	/// <summary>
	/// パイプラインステートの設定
	/// </summary>
	void PSO();

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	void SetupRootSignature();

	/// <summary>
	/// InputLayoutの設定
	/// </summary>
	void SetupInputLayout();

	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	void SetupBlendState();

	/// <summary>
	/// ラスタライザステートの設定
	/// </summary>
	void SetupRasterrizerState();

	/// <summary>
	///	シェーダをコンパイルする 
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// DepthStencilStateの設定を行なう
	/// </summary>
	void SetDepthStencilState();

	/// <summary>
	/// PSOを生成する
	/// </summary>
	void SetupPSO();

	/// <summary>
	/// CBVタイプのルートパラメータの設定と生成
	/// </summary>
	/// <param name="shaderVisibility">シェーダーの種類</param>
	/// <param name="shaderRegister">レジスタ番号</param>
	/// <returns>ルートパラメータ</returns>
	D3D12_ROOT_PARAMETER CreateCBVRootParameter(D3D12_SHADER_VISIBILITY shaderVisibility, UINT shaderRegister);

	/// <summary>
	/// シェーダーを取り込む
	/// </summary>
	/// <param name="filePath">シェーダー名</param>
	/// <param name="profile">シェーダープロファイル</param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	IDxcBlob* CompileShader
	(
		//compilerするshaderファイルへのパス
		const std::wstring& filePath,
		//Compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したものを3つ
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
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;		//RTV用のディスクリプタヒープ
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
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};	//DepthStensilStateの設定

	Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;

	//静的メンバ変数の宣言と初期化
	static DirectXCommon* instance;

};

