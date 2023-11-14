#include "Triangle.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Triangle::~Triangle()
{

}

void Triangle::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();

	SetupVertexBuffer();

	TriangleData initialData = {
	   Vector4(-0.2f, -0.2f, 0.0f, 2.0f),
	   Vector4(0.0f, 0.2f, 0.0f, 2.0f),
	   Vector4(0.2f, -0.2f, 0.0f, 2.0f)
	};

	SetVertexData(initialData);
}

void Triangle::Draw()
{
	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::SetVertexData(const TriangleData& Data)
{
	vertexData_[0] = Data.vertex[0];
	vertexData_[1] = Data.vertex[1];
	vertexData_[2] = Data.vertex[2];
}

/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

void Triangle::SetupVertexBuffer()
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


