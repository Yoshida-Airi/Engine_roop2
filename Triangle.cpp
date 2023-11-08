#include "Triangle.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Triangle::~Triangle()
{
	vertexResource_->Release();
	materialResource_->Release();
}

void Triangle::Initialize(const TriangleData& data)
{
	dxCommon_ = DirectXCommon::GetInstance();
	

	VertexBuffer();
	MaterialBuffer();


	vertexData_[0] = data.vertex[0];
	vertexData_[1] = data.vertex[1];
	vertexData_[2] = data.vertex[2];

	//色の設定
	materialData_[0] = data.color;

}

void Triangle::Draw()
{

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

}


/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

void Triangle::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(Vector4) * 3);	//頂点用のデータ

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(Vector4);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Triangle::MaterialBuffer()
{
	materialResource_ = dxCommon_-> CreateBufferResource(sizeof(Vector4));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

