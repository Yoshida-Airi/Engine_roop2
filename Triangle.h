#pragma once
#include"DirectXCommon.h"
#include"VectorMath.h"
#include"MatrixMath.h"
#include"MathUtilty.h"
#include<wrl.h>
#include"Transform.h"
#include"WorldTransform.h"


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


	WorldTransform worldTransform;

private://プライベート変数


	DirectXCommon* dxCommon_;


	Microsoft::WRL::ComPtr< ID3D12Resource> vertexResource_;	//頂点リソース
	Microsoft::WRL::ComPtr< ID3D12Resource> materialResource_;	//マテリアルリソース
	Microsoft::WRL::ComPtr< ID3D12Resource> wvpResource_;	//wvpリソース

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	VertexData* vertexData_ = nullptr;
	Vector4* materialData_ = nullptr;	//マテリアルデータ
	Matrix4x4* wvpData_ = nullptr;	//wvpデータ

	
	Transform cameraTransform_;
	

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