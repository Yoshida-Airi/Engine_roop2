#pragma once
#include"DirectXCommon.h"
#include "Externals/DirectXTex/DirectXTex.h"
#include"Externals/DirectXTex/d3dx12.h"
#include<wrl.h>
#include"Logger.h"


class TextureManager
{
public:

	//シングルトン
	static TextureManager* GetInstance();

	void Initialize();

	void CreateTexture();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	/// <summary>
	/// リソースを作成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);


	/// <summary>
	/// テクスチャリソースにデータを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	void UploadTextureData(Microsoft::WRL::ComPtr< ID3D12Resource>texture, const DirectX::ScratchImage& mipImages);

	void SetupShaderResourceView();

private:


	DirectXCommon* dxCommon_;

	//静的メンバ変数の宣言と初期化
	static TextureManager* instance;

};
