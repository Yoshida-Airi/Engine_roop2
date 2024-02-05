#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"MathUtilty.h"
#include"VectorMath.h"
#include"Transform.h"
#include"TextureManager.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"

#include<numbers>
#include<cmath>
#include<assert.h>

#include"Transform.h"

class Sphere
{
public:

	~Sphere();

	void Initialize(uint32_t textureHandle);

	void Update();

	void Draw(Camera* camera);

	WorldTransform* worldTransform_ = {};

	/// <summary>
	/// 表示するかしないか true : 表示しない
	/// </summary>
	/// <param name="isInvisible"></param>
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	/// <summary>
	/// 球体の生成
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	/// <returns>球体</returns>
	static Sphere* Create(uint32_t textureHandle);

private:

	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;

	Microsoft::WRL::ComPtr < ID3D12Resource>vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	DirectionalLight* lightData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW materialBufferView_;
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView_;

	uint32_t textureHandle_;

	//分割数
	const uint32_t kSubdivision = 16;

	//経度分割1つ分の角度
	const float kLonEvery = static_cast<float>(std::numbers::pi) * 2.0f / float(kSubdivision);
	//緯度分割1つ分の角度
	const float kLatEvery = static_cast<float>(std::numbers::pi) / float(kSubdivision);

	uint32_t latIndex = 0;
	uint32_t lonIndex = 0;

	//球の全ての頂点
	const uint32_t totalVertex = 1536;

	bool isInvisible_ = false;

private:
	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void VertexBuffer();

	/// <summary>
	/// マテリアルのバッファの取得
	/// </summary>
	void MaterialBuffer();

	/// <summary>
	/// ライトバッファ
	/// </summary>
	void LightBuffer();
};

