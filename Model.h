#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<assert.h>

#include"DirectXCommon.h"
#include"TextureManager.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"

struct MaterialData
{
	std::string textureFilePath;
};

struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
};

class Model
{
public:
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Update();
	void Draw(Camera* camera);

	/// <summary>
	/// 表示切り替え
	/// </summary>
	/// <param name="isInvisible">表示しない : true</param>
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

private:
	DirectXCommon* dxCommon_;
	TextureManager* texture_;

	WorldTransform worldTransform_;

	ModelData modelData_;	//モデル
	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	DirectionalLight* lightData_ = nullptr;

	uint32_t textureHandle_;
	bool isInvisible_ = false;

private:
	/// <summary>
	/// Objファイルを読むための関数
	/// </summary>
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// mtlファイルを読むための関数
	/// </summary>
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

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

