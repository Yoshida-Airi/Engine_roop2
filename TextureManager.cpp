#include "TextureManager.h"


TextureManager::~TextureManager()
{
	textureResource_->Release();
}

/// <summary>
/// 初期化
/// </summary>
void TextureManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	srvDescriptoHeap_ = dxCommon_->GetSRVDescriptorHeap();
	//textureを読む
	mipImages_ = LoadTexture("Resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages_.GetMetadata();
	textureResource_ = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource.at(0) = UploadTextureData(textureResource_, mipImages_);
	CreateShaderResourceView(metadata);
}

//更新処理
void TextureManager::Update()
{

}

//テクスチャデータを読む
DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath)
{
	//テクスチャファイルを読んでプログラムで扱えるようにする

	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの生成

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));


	//ミップマップ付きのデータを返す
	return mipImages;
}

//ダイレクト12のテクスチャリソースを作る
ID3D12Resource* TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	//1.metadataを基にリソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);		//Textureの幅
	resourceDesc.Height = UINT(metadata.height);	//Textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);	//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);	//奥行きor配列Textureの配列数
	resourceDesc.Format = metadata.format;	//TextureのFormat
	resourceDesc.SampleDesc.Count = 1;	//サンプリングカウント。1固定。
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	//Textureの次元数。普段使っているのは二次元


	//2.利用するヒープの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//細かい設定を行う
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;	//WriteBackポリシーでCPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;	//プロセッサの近くに配置


	//3.リソースを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,	//Heapの設定
		D3D12_HEAP_FLAG_NONE,	//Heapの特殊な設定。
		&resourceDesc,	//resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,	//初回のResourceState。Textureは基本読むだけ
		nullptr,	//Clear最適化。使わないのでNULL
		IID_PPV_ARGS(&resource));	//作成するResourceポインタへのポインタ

	assert(SUCCEEDED(hr));

	return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	std::vector<D3D12_SUBRESOURCE_DATA>subresources;
	DirectX::PrepareUpload(dxCommon_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr< ID3D12Resource> intermediateResource = dxCommon_->CreateBufferResource(intermediateSize);
	UpdateSubresources(dxCommon_->GetCommandList(), texture, intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	//Tetureへの転送後は利用できるようにD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

void TextureManager::CreateShaderResourceView(const DirectX::TexMetadata& metadata)
{
	//metaDataを基にSRVの設定
	srvDesc_.Format = metadata.format;
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc_.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = srvDescriptoHeap_->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU_ = srvDescriptoHeap_->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureResource_, &srvDesc_, textureSrvHandleCPU_);

}


