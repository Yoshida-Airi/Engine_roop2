#pragma once

#include"Logger.h"



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

	void SetupDevice();


private:

	HRESULT hr;
	IDXGIFactory7* dxgiFactory = nullptr;	//DXGIFactory
	IDXGIAdapter4* useAdapter = nullptr;	//使用するアダプタ用の変数。
};

