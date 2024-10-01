#pragma once
#include"Globals.h"
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"WorldTransform.h"

#include"Camera.h"

class Light
{
public:
	~Light();
	void Initialize();
	void Update();
	void Draw(Camera*camera);

	void SetPointLight(PointLight* pointLight)
	{
		pointLightData_ = pointLight;
	}

private:

	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;

	PointLight* pointLightData_;
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

};

