#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include<wrl.h>

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
	void Initialize(const TriangleData& position);


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="a">左下</param>
	/// <param name="b">上</param>
	/// <param name="c">右下</param>
	void Draw();



private://プライベート変数

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW materialBufferView_;

	Vector4* vertexData_ = nullptr;	//頂点データ
	Vector4* materialData_ = nullptr;	//マテリアルデータ

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
