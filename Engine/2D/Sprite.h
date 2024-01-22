#pragma once
#include"DirectXCommon.h"
#include"TextureManager.h"
#include"VectorMath.h"
#include"Globals.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include"Transform.h"
#include"WorldTransform.h"
#include<wrl.h>

#include"ICamera.h"


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

	void Initialize(uint32_t textureHandle);
	void Update();
	void Draw(ICamera* camera);

	void SetVertexData(const float left, const float right, const float top, const float bottom);

	/// <summary>
	/// マテリアルデータの設定
	/// </summary>
	/// <param name="color"></param>
	void SetMaterialData(const Vector4 color);

	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU)
	{
		textureSrvHandleGPU_ = textureSrvHandleGPU;
	}

	WorldTransform* worldTransform_;

	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}


	/// <summary>
	/// テクスチャの左上座標（画像上の描画したい左上座標
	/// </summary>
	/// <param name="leftTop"></param>
	void SetTextureLeftTop(Vector2 leftTop)
	{
		textureLeftTop = leftTop;
	}

	/// <summary>
	/// テクスチャサイズ（切り取って表示したいサイズ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(Vector2 size)
	{
		cutSize_ = size;
	}

	/// <summary>
	/// スプライトのの基準点
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(Vector2 anchorPoint)
	{
		anchorPoint_ = anchorPoint;
	}

	void SetAlpha();

	/// <summary>
	/// 四角の生成
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	/// <returns>四角形</returns>
	static Sprite* Create(uint32_t textureHandle);

	

private://プライベート変数

	DirectXCommon* dxCommon_;
	TextureManager* texture_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> indexResource_;	//頂点リソース

	D3D12_RESOURCE_DESC resourceDesc_{};	//テクスチャの情報

	Matrix4x4* transformationMatrixData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	VertexData* vertexData_ = nullptr;	//頂点データ
	Material* materialData_ = nullptr;	//マテリアルデータ
	uint32_t* indexData_ = nullptr;		//インデックスデータ

	Transform uvTransform;
	Vector2 textureSize_;	//切り出しサイズ

	bool isInvisible_ = false;	//非表示フラグ	true : 消える

	uint32_t textureHandle_;

	Vector2 cutSize_ = { 100.0f,100.0f };
	Vector2 textureLeftTop = { 0.0f,0.0f };	//テクスチャ左上座標
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	//画像のサイズ
	float left;
	float right;
	float top;
	float bottom;

	//texcoord用
	float texLeft;
	float texRight;
	float texTop;
	float texBottom;

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

	/// <summary>
	/// 頂点情報の更新
	/// </summary>
	void UpdateVertexBuffer();

	/// <summary>
	/// テクスチャサイズをオリジナルに合わせる
	/// </summary>
	void AdjustTextureSize();

};

