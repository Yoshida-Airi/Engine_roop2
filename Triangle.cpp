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
	SetupMaterialBuffer();
	SetupWVPBuffer();

	cameraTransform_ =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.1f,0.1f,0.0f}
	};

	transform =
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.1f,0.1f,0.0f}
	};

	SetupVertexBuffer();
	SetupMaterialBuffer();
	SetupWVPBuffer();

	TriangleData initData;
	initData.vertex[0] = { -0.5f,-0.5f,0.0f,1.0f };
	initData.vertex[1] = { 0.0f,0.5f,0.0f,1.0f };
	initData.vertex[2] = { 0.5f,-0.5f,0.0f,1.0f };

	initData.color = { 1.0f,0.0f,0.0f,1.0f };

	SetVertexData(initData.vertex);
	SetMaterialData(initData.color);

	*wvpData = MakeIdentity4x4();

}

void Triangle::Update()
{
	transform.rotate.y += 0.03f;
	Matrix4x4 worldMatrix = MakeAffinMatrix(transform.scale, transform.rotate, transform.translate);
	*wvpData = worldMatrix;

	Matrix4x4 cameraMatrix = MakeAffinMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
	//WVPMatrixを作る
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*wvpData = worldViewProjectionMatrix;
}

void Triangle::Draw()
{
	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::SetVertexData(const Vector4 vertex[3])
{
	//頂点の設定
	vertexData_[0].position = vertex[0];
	vertexData_[0].texcoord = { 0.0f,1.0f };

	vertexData_[1].position = vertex[1];
	vertexData_[1].texcoord = { 0.5f,0.0f };

	vertexData_[2].position = vertex[2];
	vertexData_[2].texcoord = { 1.0f,1.0f };

}

void Triangle::SetMaterialData(const Vector4 color)
{
	materialData[0] = color;
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

void Triangle::SetupMaterialBuffer()
{
	//マテリアル用のリソースを作る
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Vector4));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
}

void Triangle::SetupWVPBuffer()
{
	wvpResource_ = dxCommon_->CreateBufferResource(sizeof(Matrix4x4));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	*wvpData = MakeIdentity4x4();
}


