#pragma once

#include"Logger.h"
#include"DirectXCommon.h"

#include"DirectXTex.h"
#include"d3dx12.h"

#include<d3d12.h>
#include<array>

#pragma comment(lib,"d3d12.lib")


class TextureManager
{
public:
	static TextureManager* GetInstance();

	struct TextureData
	{
		D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
		Microsoft::WRL::ComPtr< ID3D12Resource> textureResource;
		std::string filename{};
		uint32_t textureHandle;
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <param name="filePath"></param>
	uint32_t LoadTexture(const std::string& filePath);

	//ゲッター
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index)
	{
		return textures_.at(index).textureSrvHandleGPU;
	}

	/// <summary>
	/// テクスチャの情報を取得
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <returns></returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);


private:

	static const size_t kMaxTexture = 256;	//最大テクスチャ数
	DirectXCommon* dxCommon_;

	DirectX::ScratchImage mipImages_;
	Microsoft::WRL::ComPtr< ID3D12Resource> textureResource_;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};
	Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptoHeap_;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_ = {};

	//中間リソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxTexture> intermediateResource;

	std::array<TextureData, kMaxTexture> textures_;
	bool IsusedTexture[kMaxTexture];
	uint32_t descriptorSizeSRV;

	static TextureManager* instance;

private:

	DirectX::ScratchImage ImageFileOpen(const std::string& filePath);
	Microsoft::WRL::ComPtr< ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	[[nodiscard]]
	Microsoft::WRL::ComPtr< ID3D12Resource> UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


	//uint32_t FindOrCreateTextureIndex(const std::string& filePath);

};
