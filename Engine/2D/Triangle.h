#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"VectorMath.h"
#include"Globals.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"
#include"WorldTransform.h"

#include"Camera.h"


#include<wrl.h>

struct TriangleData
{
	Vector4 vertex[3];
	Vector4 color;
};

class Triangle
{
public:
	~Triangle();

	void Initialize(uint32_t textureHandle);

	void Update();
	void Draw(Camera* camera);


	/// <summary>
	/// マテリアルデータの設定
	/// </summary>
	/// <param name="color"></param>
	void SetMaterialData(const Vector4 color);

	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU)
	{
		textureSrvHandleGPU_ = textureSrvHandleGPU;
	}

	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	WorldTransform* worldTransform_;

	/// <summary>
	/// 三角形の生成
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	/// <returns>三角形</returns>
	static Triangle* Create(uint32_t textureHandle);
	
private://プライベート変数

	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;

	
	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> wvpResource_;	//wvpリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	//Matrix4x4* wvpData_ = nullptr;	//wvpデータ
	DirectionalLight* lightData_ = nullptr;

	Transform cameraTransform_;
	
	uint32_t textureHandle_;

	

	bool isInvisible_ = false;

private://プライベート関数

	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void VertexBuffer();

	/// <summary>
	/// マテリアルのバッファの取得
	/// </summary>
	void MaterialBuffer();

	/// <summary>
	/// ライトのバッファの取得
	/// </summary>
	void LightBuffer();
};

