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


class Skybox
{
public:
	~Skybox();
	void Initialize(uint32_t textureHandle);
	void Update();
	void Draw(Camera* camera);


	/// <summary>
	/// 表示するかしないか true : 表示しない
	/// </summary>
	/// <param name="isInvisible"></param>
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	WorldTransform* GetWorldTransform()const { return worldTransform_; };

	/// <summary>
	/// 球体の生成
	/// </summary>
	/// <param name="textureHandle">テクスチャ番号</param>
	/// <returns>球体</returns>
	static Skybox* Create(uint32_t textureHandle);

private:

	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;

	Microsoft::WRL::ComPtr < ID3D12Resource>vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	DirectionalLight* lightData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW materialBufferView_;
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	uint32_t textureHandle_;

	WorldTransform* worldTransform_ = {};

	bool isInvisible_ = false;

	const uint32_t totalVertex = 24;

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

	/// <summary>
	/// インデックス
	/// </summary>
	void IndexBuffer();

	/// <summary>
	/// インデックスデータの書き込み
	/// </summary>
	void IndexData();


};

