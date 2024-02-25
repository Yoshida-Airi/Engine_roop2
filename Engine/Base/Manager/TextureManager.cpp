#include "TextureManager.h"

uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager;
	}
	return instance;
}

TextureManager::~TextureManager()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void TextureManager::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	srvDescriptoHeap_ = dxCommon_->GetSRVDescriptorHeap();
	descriptorSizeSRV = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	textureDatas.reserve(DirectXCommon::kMaxSRVCount);

}

//更新処理
void TextureManager::Update()
{

}


uint32_t TextureManager::LoadTexture(const std::string& filePath)
{
	uint32_t index = 0;


	//読み込み済みテクスチャを検索
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filename == filePath; }
	);
	if (it != textureDatas.end())
	{
		//読み込み済みなら戻す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}

	for (int i = 0; i < kMaxTexture; i++)
	{
		if (IsusedTexture[i] == false) {
			index = i;
			IsusedTexture[i] = true;
			break;
		}
	}

	//テクスチャ枚数上限
	assert(textureDatas.size() + kSRVIndexTop < DirectXCommon::kMaxSRVCount);

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = ImageFileOpen(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	
	textureDatas.resize(textureDatas.size() + 1);
	TextureData& textureData = textureDatas.back();

	textureData.filename = filePath;
	textureData.textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);

	textureData.textureResource = CreateTextureResource(dxCommon_->GetDevice(), metadata);
	intermediateResource.at(index) = UploadTextureData(textureData.textureResource.Get(), mipImages);


	//テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.textureSrvHandleCPU = GetCPUDescriptorHandle(srvDescriptoHeap_, descriptorSizeSRV, srvIndex);
	textureData.textureSrvHandleGPU = GetGPUDescriptorHandle(srvDescriptoHeap_, descriptorSizeSRV, srvIndex);

	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVの作成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.textureResource.Get(), &srvDesc, textureData.textureSrvHandleCPU);

	return index;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvGPUHandle(uint32_t textureIndex)
{
	//範囲外指定違反の場合止める
	assert(textureIndex < DirectXCommon::kMaxSRVCount);

	TextureData& textureData = textureDatas.at(textureIndex);
	return textureData.textureSrvHandleGPU;
}



const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle)
{

	//範囲外指定違反の場合止める
	assert(textureHandle < DirectXCommon::kMaxSRVCount);

	TextureData& textureData = textureDatas.at(textureHandle);

	//テクスチャの情報を取得
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc = textureDatas.at(textureHandle).textureResource.Get()->GetDesc();

	return resourceDesc;
}


DirectX::ScratchImage TextureManager::ImageFileOpen(const std::string& filePath)
{
	//テクスチャファイルを読み込みプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));
	//ミップマップの作成
	DirectX::ScratchImage mipImage{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);

	return mipImage;
}

//ダイレクト12のテクスチャリソースを作る
Microsoft::WRL::ComPtr< ID3D12Resource> TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
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
	

	//3.リソースを生成する
	Microsoft::WRL::ComPtr< ID3D12Resource> resource = nullptr;
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

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

TextureManager* TextureManager::instance = NULL;