#include "Triangle.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Triangle::~Triangle()
{
	vertexResource_->Release();
	materialResource_->Release();
	wvpResource_->Release();
}

void Triangle::Initialize(const TriangleData& data)
{

	dxCommon_ = DirectXCommon::GetInstance();


	cameraTransform_ =
	{
		{1.0f, 1.0f, 1.0f },
		{0.0f, 0.0f, 0.0f },
		{0.0f, 0.0f, -5.0f}
	};

	transform_ =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.1f,0.1f,0.0f}
	};

	VertexBuffer();

	MaterialBuffer();
	WvpBuffer();



	//頂点の設定
	vertexData_[0].position = data.vertex[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = data.vertex[1];
	vertexData_[1].texcoord = { 0.5f,0.0f };

	vertexData_[2].position = data.vertex[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

	//色の設定
	materialData_[0] = { 1.0f,0.0f,0.0f,1.0f };


	*wvpData_ = MakeIdentity4x4();

}

void Triangle::Update()
{
	transform_.rotate.y += 0.03f;
	Matrix4x4 worldMatrix = MakeAffinMatrix(transform_.scale, transform_.rotate, transform_.translate);
	*wvpData_ = worldMatrix;

	Matrix4x4 cameraMatrix = MakeAffinMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
	//WVPMatrixを作る
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*wvpData_ = worldViewProjectionMatrix;


}


void Triangle::Draw()
{
	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
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
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Vector4));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Triangle::WvpBuffer()
{
	//wvp用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	*wvpData_ = MakeIdentity4x4();
}
