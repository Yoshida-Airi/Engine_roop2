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
#include"SpriteGlobals.h"

#include<random>

struct ParticleData
{
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct Emitter
{
	Transform transform;	//エミッタのTransform
	uint32_t count;			//発生数
	float frequency;		//発生頻度
	float frequencyTime;	//頻度用時刻
};

struct ParticleVertexData
{
	Vector4 position;
	Vector2 texcoord;
};

struct ParticleForGPU
{
	Matrix4x4 worldMatrix;
	Vector4 color;
};

class Particle
{
public:
	~Particle();

	void Initialize(uint32_t textureHandle, Emitter emitter);

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

	
	

	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	/// <summary>
	/// テクスチャサイズ（描画したい範囲
	/// </summary>
	/// <param name="textureSize"></param>
	void SetTextureSize(Vector2 textureSize)
	{
		textureSize_ = textureSize;
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
	/// テクスチャサイズ（表示したいサイズ
	/// </summary>
	/// <param name="size"></param>
	void SetSize(Vector2 size)
	{
		size_ = size;
	}

	/// <summary>
	/// スプライトのの基準点
	/// </summary>
	/// <param name="anchorPoint"></param>
	void SetAnchorPoint(Vector2 anchorPoint)
	{
		anchorPoint_ = anchorPoint;
	}

	void Debug();

	std::list<ParticleData>Emission(const Emitter& emitter, std::mt19937& randomEngine);

	static Particle* Create(uint32_t textureHandle, Emitter emitter);

private://プライベート変数

	static const uint32_t kNumMaxInstance = 10;

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

	ParticleVertexData* vertexData_ = nullptr;	//頂点データ
	Material* materialData_ = nullptr;	//マテリアルデータ
	uint32_t* indexData_ = nullptr;		//インデックスデータ

	Transform uvTransform;
	Vector2 textureSize_;	//切り出しサイズ

	bool isInvisible_ = false;	//非表示フラグ	true : 消える

	uint32_t textureHandle_;
	Emitter emitter_;

	Vector2 size_ = { 100.0f,100.0f };
	Vector2 textureLeftTop = { 0.0f,0.0f };
	Vector2 anchorPoint_ = { 0.0f,0.0f };


	float left;
	float right;
	float top;
	float bottom;

	float texLeft;
	float texRight;
	float texTop;
	float texBottom;


	Microsoft::WRL::ComPtr<ID3D12Resource>instancingResource;
	ParticleForGPU* instancingData;
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

	std::list <ParticleData> particles;
	const float kDeltaTime = 1.0f / 60.0f;

	

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

	void instancingBuffer();
	void SetSRV();


	void AdjustTextureSize();

	/// <summary>
	/// パーティクル生成関数
	/// </summary>
	/// <param name="randomEngine">ランダム関数</param>
	/// <returns>パーティクル</returns>
	ParticleData MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate);

};
