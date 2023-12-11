#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include"VertexData.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include"Transform.h"
#include"WorldTransform.h"
#include<wrl.h>

#include"UICamera.h"


struct SpriteData
{
	Vector4 vertex[4];
	Vector4 color;
};

struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
};


enum VertexNumber
{
	LB,
	LT,
	RB,
	RT
};


class Sprite
{
public:
	~Sprite();

	void Initialize();

	void Update();
	void Draw(UICamera* camera);

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

	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

private://プライベート変数

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	
	Microsoft::WRL::ComPtr< ID3D12Resource> indexResource_;	//頂点リソース

	Matrix4x4* transformationMatrixData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	VertexData* vertexData_ = nullptr;	//頂点データ
	Vector4* materialData_ = nullptr;	//マテリアルデータ
	uint32_t* indexData_ = nullptr;		//インデックスデータ

	Transform transform_ = {};
	
	bool isInvisible_ = false;	//非表示フラグ	true : 消える

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
	/// index用
	/// </summary>
	void IndexBuffer();

};

