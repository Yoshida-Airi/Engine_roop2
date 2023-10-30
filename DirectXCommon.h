#pragma once

#include"Logger.h"
#include"WinApp.h"


#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectXCommon
{
public:
	void Initialize();
	void Update();
	void Draw();

private:

	/// <summary>
	/// DXGIFactoryの生成
	/// </summary>
	void SetupDXGIFactory();

	/// <summary>
	/// アダプタ(GPU)を見つける
	/// </summary>
	void SelectOptimalGPU();

	/// <summary>
	/// デバイスの生成
	/// </summary>
	void SetupDevice();

	/// <summary>
	/// コマンドキューの生成
	/// </summary>
	void SetupCommandQueue();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	void SetupCommandList();

	/// <summary>
	///　スワップチェインの生成
	/// </summary>
	void SetupSwapChain();

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	void SetupDescriptorHeap();

private:
	
	WinApp* winApp_;

	HRESULT hr;
	IDXGIFactory7* dxgiFactory = nullptr;	//DXGIFactory
	IDXGIAdapter4* useAdapter = nullptr;	//使用するアダプタ用の変数。
	ID3D12Device* device = nullptr;			//デバイス
	ID3D12CommandQueue* commandQueue = nullptr;	//コマンドキュー

};

