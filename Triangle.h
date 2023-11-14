#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include<wrl.h>

struct TriangleData
{
	Vector4 vertex[3];
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
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="a">左下</param>
	/// <param name="b">上</param>
	/// <param name="c">右下</param>
	void Draw();

	/// <summary>
	/// トライアングルデータの変更
	/// </summary>
	/// <param name="Data">トライアングルデータ</param>
	void SetVertexData(const TriangleData& Data);

private://プライベート変数

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Vector4* vertexData_ = nullptr;	//頂点データ

private://プライベート関数

	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void SetupVertexBuffer();

};
