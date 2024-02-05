#pragma once
#include"Logger.h"

#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<chrono>
#include<dxcapi.h>

#include<wrl.h>

#pragma comment(lib,"dxcompiler.lib")

#include"DirectXCommon.h"

class GraphicsPipelineManager
{
public:

	struct PSOData
	{
		Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
		Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;
		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成
		Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState = nullptr;
	};

	void Initialize();

	//シングルトン
	static GraphicsPipelineManager* GetInstance();

	ID3D12RootSignature* GetRootSignature()const { return rootSignature.Get(); };
	ID3D12PipelineState* GetGraphicPipelineState()const { return graphicPipelineState.Get(); };

private:

	DirectXCommon* dxCommon_ = nullptr;
	HRESULT hr;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;

	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	D3D12_BLEND_DESC NormalblendDesc{};

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};	//DepthStensilStateの設定

	Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState = nullptr;
	
	//静的メンバ変数の宣言と初期化
	static GraphicsPipelineManager* instance;

private:

	
	PSOData CreatePSO();

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXCCompiler();


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
	/// 深度
	/// </summary>
	void SetupDepthStencilState();

	/// <summary>
	/// シェーダーコンパイル用
	/// </summary>
	/// <param name="filePath">シェーダー名</param>
	/// <param name="profile"></param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandler"></param>
	/// <returns></returns>
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler);


};

