#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"
#include"WorldTransform.h"

#include"Camera.h"


struct  VertexData
{
	Vector4 position;
	Vector2 texcoord;
};


struct TriangleData
{
	Vector4 vertex[3];
	Vector4 color;
};


class Triangle
{
public:

	~Triangle();

	void Initialize();

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

	WorldTransform worldTransform;


private://プライベート変数


	DirectXCommon* dxCommon_;
	



	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> wvpResource_;	//wvpリソース

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	VertexData* vertexData_ = nullptr;
	Vector4* materialData_ = nullptr;	//マテリアルデータ
	//Matrix4x4* wvpData_ = nullptr;	//wvpデータ

	Transform cameraTransform_;
	


private://プライベート関数

	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void VertexBuffer();

	/// <summary>
	/// マテリアルのバッファの取得
	/// </summary>
	void MaterialBuffer();




};

