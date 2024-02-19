#pragma once
#include"VectorMath.h"
#include"DirectXCommon.h"
#include<wrl.h>

class DirectionalLight
{
public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};


public:
	void Initialize();
	void Update();
	void Draw();


	ID3D12Resource* GetLight()
	{
		return lightResource_.Get();
	}

private:
	DirectXCommon* dxCommon_ = nullptr;

	// ライト方向（単位ベクトル）
	Vector3 lightDir_ = { 1, 0, 0 };
	// ライト色
	Vector4 lightColor_ = { 1, 1, 1 };
	// 有効フラグ
	bool active_ = false;


	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;
	ConstBufferData* lightData_ = nullptr;

private:
	void LightBuffer();

};

