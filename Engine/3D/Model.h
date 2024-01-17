#pragma once
#include"DirectXCommon.h"
#include"TextureManager.h"
#include"ModelLoader.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"ICamera.h"
#include"ModelData.h"

class Model
{
public:
	~Model();
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Update();
	void Draw(ICamera* camera);

	/// <summary>
	/// 表示切り替え
	/// </summary>
	/// <param name="isInvisible">表示しない : true</param>
	void SetisInvisible(bool isInvisible)
	{
		isInvisible_ = isInvisible;
	}

	WorldTransform* worldTransform_;

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="data">モデルデータ</param>
	/// <returns>モデル</returns>
	static std::unique_ptr< Model>  Create(const std::string& directoryPath, const std::string& filename);

	void ModelDebug(const char title[10]);

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	/// <param name="model">親のモデル</param>
	void Parent(Model* model);

private:
	DirectXCommon* dxCommon_;
	TextureManager* texture_;
	ModelLoader* modelLoader_;

	ModelData modelData_;

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

