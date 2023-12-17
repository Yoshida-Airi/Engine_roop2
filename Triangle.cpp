#include "Triangle.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Triangle::~Triangle()
{

}

void Triangle::Initialize(uint32_t textureHandle)
{

	dxCommon_ = DirectXCommon::GetInstance();
	texture_ = TextureManager::GetInstance();
	worldTransform.Initialize();
	textureHandle_ = textureHandle;

	VertexBuffer();
	MaterialBuffer();

	TriangleData initData;
	initData.vertex[0] = { -0.5f,-0.5f,0.0f,1.0f };
	initData.vertex[1] = { 0.0f,0.5f,0.0f,1.0f };
	initData.vertex[2] = { 0.5f,-0.5f,0.0f,1.0f };

	initData.color = { 1.0f,1.0f,1.0f,1.0f };

	//頂点の設定
	vertexData_[0].position = initData.vertex[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = initData.vertex[1];
	vertexData_[1].texcoord = { 0.5f,0.0f };

	vertexData_[2].position = initData.vertex[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

	SetMaterialData(initData.color);

	materialData_->uvTransform = MakeIdentity4x4();

}

void Triangle::Update()
{
	worldTransform.UpdateWorldMatrix();

}

void Triangle::Draw(ICamera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer_->GetGPUVirtualAddress());
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetGPUHandle(textureHandle_));
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}


void Triangle::SetMaterialData(const Vector4 color)
{
	materialData_[0].color = color;
}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

void Triangle::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 3);	//頂点用のデータ

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Triangle::MaterialBuffer()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}
