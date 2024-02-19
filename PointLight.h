#pragma once
#include"VectorMath.h"
#include"DirectXCommon.h"

class PointLight
{
public:

	struct PointLightData
	{
		Vector4 color;
		Vector3 position;
		float intensity;
	};

public:
	void Initialize();
	void Update();
	void Draw();

	ID3D12Resource

private:

	DirectXCommon* dxCommon_ = nullptr;
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;
	PointLightData lightData;
};

