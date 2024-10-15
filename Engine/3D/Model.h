#pragma once
#include"DirectXCommon.h"
#include"GraphicsPipelineManager.h"
#include"TextureManager.h"
#include"ModelLoader.h"
#include"Globals.h"
#include"WorldTransform.h"
#include"Camera.h"
#include"ModelData.h"
#include"Animation.h"
#include"SrvManager.h"
#include<span>

const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence
{
	std::array<float, kNumMaxInfluence>weights;
	std::array<int32_t, kNumMaxInfluence>jointIndices;
};

struct WellForGPU
{
	Matrix4x4 skeletonSpaceMatrix;	//位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;	//法線用
};

struct SkinCluster
{
	std::vector<Matrix4x4>inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource>influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence>mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource>palatteResource;
	std::span<WellForGPU>mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, CD3DX12_GPU_DESCRIPTOR_HANDLE>paletteSrvHandle;
};


class Model
{
public:
	~Model();
	void Initialize(const std::string& filename);
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

	void UseAnimation(bool isUse)
	{
		animation.isValid = isUse;
	}

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="filename">ファイルパス名</param>
	/// <returns>モデル</returns>
	static Model* Create(const std::string& filename);

	void ModelDebug(const char* name);

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	/// <param name="model">親のモデル</param>
	void Parent(Model* model);

	WorldTransform* GetWorldTransform()const { return worldTransform_; };

	void SetWorldTransform(WorldTransform* worldTransform)
	{
		worldTransform_ = worldTransform;
	}
	
	void SetAnimation(AnimationData animationData)
	{
		animation = animationData;
	}

	void MoveAnimation(bool isActive)
	{
		isActiveAnimation = isActive;
	}

	void SetMaterial(Vector4 material)
	{
		materialData_->color = material;
	}

	void SetLight(uint32_t isLight)
	{
		materialData_->enableLighting = isLight;
	}

private:
	DirectXCommon* dxCommon_;
	GraphicsPipelineManager* psoManager_;
	TextureManager* texture_;
	ModelLoader* modelLoader_;
	Animation* animation_;
	SrvManager* srvManager_;

	WorldTransform* worldTransform_;

	ModelData modelData_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr < ID3D12Resource> lightResource_;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;


	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	VertexData* vertexData_ = nullptr;
	Material* materialData_ = nullptr;	//マテリアルデータ
	DirectionalLight* lightData_ = nullptr;
	uint32_t* indexData_ = nullptr;

	uint32_t textureHandle_;
	bool isInvisible_ = false;


	AnimationData animation;
	Skeleton skelton;
	SkinCluster skinCluster;
	float animationTime = 0.0f;

	bool isActiveAnimation = true;

	
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

	void IndexBuffer();


	void UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton);

	/// <summary>
	/// スキンクラスターの生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="skeleton"></param>
	/// <param name="modelData"></param>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <returns></returns>
	SkinCluster CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton/*, const ModelData& modelData*/);

	void ClasterUpdate(SkinCluster& skinCluster, const Skeleton& skeltion);

};

