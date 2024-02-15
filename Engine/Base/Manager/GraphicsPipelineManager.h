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

enum BlendMode
{
	kBlendModeNone,		//ブレンドなし
	kBlendModeNormal,	//通常のブレンド
	kBlendModeAdd,		//加算
	kBlendModeSubtract,	//減算
	kBlendModeMultily,	//乗算
	kBlendModeScreen,	//スクリーン
};

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

	struct PsoMember
	{
		PSOData object3D;
		PSOData sprite;
		PSOData particle;
	};



	void Initialize();

	//シングルトン
	static GraphicsPipelineManager* GetInstance();

	PsoMember GetPsoMember()const { return psoMember; };

private:

	DirectXCommon* dxCommon_ = nullptr;
	HRESULT hr;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;

	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature = nullptr;	//バイナリを元に生成


	D3D12_BLEND_DESC NormalblendDesc{};

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	Microsoft::WRL::ComPtr< IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr< IDxcBlob> pixelShaderBlob;

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};	//DepthStensilStateの設定

	Microsoft::WRL::ComPtr< ID3D12PipelineState> graphicPipelineState = nullptr;
	
	PsoMember psoMember;

	//静的メンバ変数の宣言と初期化
	static GraphicsPipelineManager* instance;

private:

	
	PSOData CreateObject3D(const std::wstring& filePath);
	PSOData CreateSprite(const std::wstring& filePath);
	PSOData CreateParticle(const std::wstring& filePath);

	/// <summary>
	/// 全体のPSO生成関数
	/// </summary>
	/// <param name="filePath">シェーダーファイル</param>
	/// <param name="rootParameters">ルートパラメータ</param>
	/// <param name="numRootParameters">ルートパラメーターの配列数</param>
	/// <returns>PSOData</returns>
	GraphicsPipelineManager::PSOData CreateCommonPSO
	(
		const std::wstring& filePath, 
		D3D12_ROOT_PARAMETER* rootParameters, 
		int numRootParameters,
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc
	);

	/// <summary>
	/// DXCの初期化
	/// </summary>
	void InitializeDXCCompiler();


	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	void SetupBlendState(BlendMode blendMode);

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

