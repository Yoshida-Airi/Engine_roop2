#include "Sprite.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Sprite::~Sprite()
{

}

void Sprite::Initialize()
{

	dxCommon_ = DirectXCommon::GetInstance();
	worldTransform.Initialize();

	VertexBuffer();
	MaterialBuffer();
	WvpBuffer();

	transform_ = {
		{1.0f,1.0f,1.0f,},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	SpriteData initData;
	initData.vertex[0] = { 0.0f,360.0f,0.0f,1.0f };
	initData.vertex[1] = { 0.0f,0.0f,0.0f,1.0f };
	initData.vertex[2] = { 640.0f,360.0f,0.0f,1.0f };
	initData.vertex[3] = { 0.0f,0.0f,0.0f,1.0f };
	initData.vertex[4] = { 640.0f,0.0f,0.0f,1.0f };
	initData.vertex[5] = { 640.0f,360.0f,0.0f,1.0f };

	initData.color = { 1.0f,1.0f,1.0f,1.0f };

	//頂点の設定
	vertexData_[0].position = initData.vertex[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = initData.vertex[1];
	vertexData_[1].texcoord = { 0.0f,0.0f };

	vertexData_[2].position = initData.vertex[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

	vertexData_[3].position = initData.vertex[3];
	vertexData_[3].texcoord = { 0.0f,0.0f };

	vertexData_[4].position = initData.vertex[4];
	vertexData_[4].texcoord = { 1.0f,0.0f };

	vertexData_[5].position = initData.vertex[5];
	vertexData_[5].texcoord = { 1.0f,1.0f };

	SetMaterialData(initData.color);
}

void Sprite::Update()
{
	Matrix4x4 worldmatrix = MakeAffinMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicmatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldmatrix, Multiply(viewMatrix, projectionMatrix));
	*transformationMatrixData = worldViewProjectionMatrix;
}

void Sprite::Draw(Camera* camera)
{
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationmatrixResource->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

void Sprite::SetMaterialData(const Vector4 color)
{
	materialData_[0] = color;
}

/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/

void Sprite::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 6);	//頂点用のデータ

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Sprite::MaterialBuffer()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Vector4));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}

void Sprite::WvpBuffer()
{
	//トランスフォーメーションマトリックス用のリソースを作る
	transformationmatrixResource = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
	//書き込むためのアドレスを取得
	transformationmatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	//単位行列を書き込んでおく
	*transformationMatrixData = MakeIdentity4x4();
}
