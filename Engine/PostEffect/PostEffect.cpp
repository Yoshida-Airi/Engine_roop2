#include "PostEffect.h"

void PostEffect::Initialize()
{
	dxCommon = DirectXCommon::GetInstance();
	srvManager = SrvManager::GetInstance();
	psoManager = GraphicsPipelineManager::GetInstance();

	CreateRTV();

	srvHandle = 100;
	srvManager->CreateSRVforTexture2D(srvHandle, renderTextureResource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);

	CreateDSV();
	


	SetupViewport();
	SetupScissor();
}

void PostEffect::Update()
{
}

void PostEffect::PreDraw()
{
	// リソースの状態をログに記録
	OutputDebugString(L"RenderPreDraw: Transitioning renderTextureResource to RENDER_TARGET\n");


	//今回のバリアはTransition
	renderBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	renderBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象リソース。現在のバッグバッファに対して行う
	renderBarrier.Transition.pResource = renderTextureResource.Get();
	//遷移前(現在)のResourceState
	renderBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//遷移後のResourceState
	renderBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//TransitionBarrierを張る
	dxCommon->GetCommandList()->ResourceBarrier(1, &renderBarrier);

	//描画用のRTVとDSVを設定する
	dsvhandle = dxCommon->GetDSV()->GetCPUDescriptorHandleForHeapStart();



	dxCommon->GetCommandList()->OMSetRenderTargets(1, &renderRtvHandle, false, &dsvhandle);
	//指定した色で画面全体をクリアする
	float clearValue[] = { 1.0f,0.0f,0.0f,1.0f };//赤色。RGBAの順
	dxCommon->GetCommandList()->ClearRenderTargetView(renderRtvHandle, clearValue, 0, nullptr);
	dxCommon->GetCommandList()->ClearDepthStencilView(dsvhandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	dxCommon->GetCommandList()->RSSetViewports(1, &viewport);
	dxCommon->GetCommandList()->RSSetScissorRects(1, &scissorRect);
	

	
}

void PostEffect::PostDraw()
{
	OutputDebugString(L"RenderPostDraw: Transitioning renderTextureResource to PIXEL_SHADER_RESOURCE\n");


	renderBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	renderBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	dxCommon->GetCommandList()->ResourceBarrier(1, &renderBarrier);
}

void PostEffect::Draw()
{

	//コピー処理
	dxCommon->GetCommandList()->SetGraphicsRootSignature(psoManager->GetPsoMember().copyImage.rootSignature.Get());
	dxCommon->GetCommandList()->SetPipelineState(psoManager->GetPsoMember().copyImage.graphicPipelineState.Get());
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvManager->GetGPUDescriptorHandle(srvHandle));
	////頂点３つ描画
	dxCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}


Microsoft::WRL::ComPtr<ID3D12Resource> PostEffect::CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;		//Textureの幅
	resourceDesc.Height = height;	//Textureの高さ
	resourceDesc.MipLevels = 1;		//mipmapの数
	resourceDesc.DepthOrArraySize = 1;	//奥行き　or　配列Textureの配列数
	resourceDesc.Format = format;	//DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;		//サンプリングカウント　1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//VRAM上に作る

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;


	//Resourceの生成
	Microsoft::WRL::ComPtr< ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,	//Heaoの設定
		D3D12_HEAP_FLAG_NONE,	//heapの特殊な設定。
		&resourceDesc,	//Resourceの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,	//Clear最適値
		IID_PPV_ARGS(&resource)	//作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	return resource;
}


Microsoft::WRL::ComPtr<ID3D12Resource> PostEffect::CreateDepthStencilTextureResource(int32_t width, int32_t height)
{
	//生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width;		//Textureの幅
	resourceDesc.Height = height;	//Textureの高さ
	resourceDesc.MipLevels = 1;		//mipmapの数
	resourceDesc.DepthOrArraySize = 1;	//奥行き　or　配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1;		//サンプリングカウント　1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//DepthStencilとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;	//VRAM上に作る

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//フォーマット。resourceと合わせる

	//Resourceの生成
	Microsoft::WRL::ComPtr< ID3D12Resource> resource = nullptr;
	HRESULT hr = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProperties,	//Heaoの設定
		D3D12_HEAP_FLAG_NONE,	//heapの特殊な設定。
		&resourceDesc,	//Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//深度値を書き込む状態にしておく
		&depthClearValue,	//Clear最適値
		IID_PPV_ARGS(&resource)	//作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	return resource;
}

void PostEffect::CreateRTV()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	//３つ目のディスクリプタハンドルを得る(自力で)
	renderRtvHandle = dxCommon->GetRTV()->GetCPUDescriptorHandleForHeapStart();
	renderRtvHandle.ptr += (dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * 2);
	renderTextureResource = CreateRenderTextureResource(dxCommon->GetDevice(), WinApp::kCilentWidth, WinApp::kCilentHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	dxCommon->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, renderRtvHandle);


}

void PostEffect::CreateDSV()
{


	depthStencilResource = CreateDepthStencilTextureResource(WinApp::kCilentWidth, WinApp::kCilentHeight);


	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//Format 基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;//2dTexture
	//DSVHeapの先頭にDSVを作る
	dxCommon->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dxCommon->GetDSV()->GetCPUDescriptorHandleForHeapStart());

}


void PostEffect::SetupViewport()
{
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WinApp::kCilentWidth;
	viewport.Height = WinApp::kCilentHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void PostEffect::SetupScissor()
{
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kCilentWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kCilentHeight;
}