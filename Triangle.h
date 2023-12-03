#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"

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

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Triangle();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="direct"></param>
	void Initialize(const TriangleData& data);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="a">左下</param>
	/// <param name="b">上</param>
	/// <param name="c">右下</param>
	void Draw();

	// Setter
	void SetTextureSrvHandleGPU(D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU);
private://プライベート変数

	
	DirectXCommon* dxCommon_;
	

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> wvpResource_;	//wvpリソース

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW materialBufferView_;
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView_;


	VertexData* vertexData_ = nullptr;
	Vector4* materialData_ = nullptr;	//マテリアルデータ
	Matrix4x4* wvpData_ = nullptr;	//wvpデータ

	Transform transform_;
	Matrix4x4 worldMatrix_;

	Transform cameraTransform_;


	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

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
	/// wvpのバッファの取得
	/// </summary>
	void WvpBuffer();


};
