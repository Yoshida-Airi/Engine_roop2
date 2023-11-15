#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"

struct TriangleData
{
	Vector4 vertex[3];	//頂点座標
	Vector4 color;		//色
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
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 頂点データの変更
	/// </summary>
	/// <param name="Data">トライアングルデータ</param>
	void SetVertexData(const Vector4 vertex[3]);

	/// <summary>
	/// マテリアルデータの設定
	/// </summary>
	/// <param name="color"></param>
	void SetMaterialData(const Vector4 color);

private://プライベート変数

	DirectXCommon* dxCommon_;

	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	Vector4* vertexData_ = nullptr;	//頂点データ

	Microsoft::WRL::ComPtr<ID3D12Resource>materialResource_;	//マテリアルリソース
	Vector4* materialData = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>wvpResource_;
	Matrix4x4* wvpData = nullptr;
	Transform transform;



private://プライベート関数

	/// <summary>
	/// 頂点のバッファの取得
	/// </summary>
	void SetupVertexBuffer();

	/// <summary>
	/// マテリアルバッファの生成
	/// </summary>
	void SetupMaterialBuffer();

	/// <summary>
	/// WVPバッファの生成
	/// </summary>
	void SetupWVPBuffer();

};
