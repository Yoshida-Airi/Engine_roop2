#pragma once
#include<d3d12.h>
#include"WinApp.h"
#include"DirectXCommon.h"
#include"VectorMath.h"
#include<wrl.h>
#include"SrvManager.h"
#include"GraphicsPipelineManager.h"


class PostEffect
{
public:

	~PostEffect();
	void Initialize();
	void Update();
	void PreDraw();
	void PostDraw();
	void Draw();


	Microsoft::WRL::ComPtr<ID3D12Resource>CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device>device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);

	void CreateRTV();
	void CreateDSV();

	void SetupViewport();

	void SetupScissor();


private:

	DirectXCommon* dxCommon = nullptr;
	SrvManager* srvManager = nullptr;
	GraphicsPipelineManager* psoManager = nullptr;

	const Vector4 kRenderTargetClearValue = { 1.0f,0.0f,0.0f,1.0f };
	D3D12_CPU_DESCRIPTOR_HANDLE renderRtvHandle;	//RTVを二つ作るのでディスクリプタを二つ用意
	D3D12_RESOURCE_BARRIER barrier{};	//トランスフォームバリア
	Microsoft::WRL::ComPtr < ID3D12Resource> renderTextureResource = { nullptr };

	D3D12_VIEWPORT viewport{};	//ビューポート
	D3D12_RECT scissorRect{};	//シザー矩形

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;		//DSV用のディスクリプタヒープ
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvhandle;
	uint32_t srvHandle;
};

