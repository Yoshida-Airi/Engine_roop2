#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Transform.h"
#include"MatrixMath.h"

#include<wrl.h>

class ICamera
{
public:
    virtual void Initialize() = 0;
    virtual void CameraDebug() = 0;

    WinApp* winApp;
    DirectXCommon* dxCommon;

    Transform transform;
    Matrix4x4 matView;
    Matrix4x4 matProjection;


    // 定数バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffer_;

    virtual void CreateConstBuffer() = 0;
    virtual void Map() = 0;
    virtual void UpdateMatrix() = 0;
    virtual void TransferMatrix() = 0;
    virtual void UpdateViewMatrix() = 0;
    virtual void UpdateProjectionMatrix() = 0;

};