#pragma once
#include<wrl.h>
#include<d3d12.h>

#include<dxgi1_6.h>
#include<dxgidebug.h>



class DebugHelper
{
public:
   
    static void SetupDebugLayer();
    static void ConfigDebugMessageFilter(Microsoft::WRL::ComPtr< ID3D12Device> device);
    static void ReportLiveObjects();

private:
 
};