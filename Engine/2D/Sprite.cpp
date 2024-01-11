#include "Sprite.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Sprite::~Sprite()
{

}

void Sprite::Initialize(uint32_t textureHandle)
{

	dxCommon_ = DirectXCommon::GetInstance();
	texture_ = TextureManager::GetInstance();

	
	textureHandle_ = textureHandle;

	resourceDesc_ = texture_->GetResourceDesc(textureHandle_);
	
	textureSize_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };
	size_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };

	uvTransform =
	{
		{1.0f,1.0f,1.0f,},
		{0.0f,0.0f,0.0f,},
		{0.0f,0.0f,0.0f},
	};

	VertexBuffer();
	MaterialBuffer();
	IndexBuffer();
	instancingBuffer();

	SetSRV();

	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		worldTransform[index].Initialize();
		worldTransform[index].scale_ = { 1.0f,1.0f,1.0f };
		worldTransform[index].rotation_ = { 0.0f,0.0f,0.0f };
		worldTransform[index].translation_ = { index * 10.0f,index * 10.0f,index * 10.0f };
		
	}


	left = 0.0f * size_.x;
	right = 1.0f * size_.x;
	top = 0.0f * size_.y;
	bottom = 1.0f * size_.y;

	texLeft = textureLeftTop.x / resourceDesc_.Width;
	texRight = (textureLeftTop.x + textureSize_.x) / resourceDesc_.Width;
	texTop = textureLeftTop.y / resourceDesc_.Height;
	texBottom = (textureLeftTop.y + textureSize_.y) / resourceDesc_.Height;


	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	//頂点の設定
	vertexData_[LB].position = { left,bottom,0.0f,1.0f };
	vertexData_[LB].texcoord = { texLeft,texBottom };

	vertexData_[LT].position = { left,top,0.0f,1.0f };
	vertexData_[LT].texcoord = { texLeft,texTop };

	vertexData_[RB].position = { right,bottom,0.0f,1.0f };
	vertexData_[RB].texcoord = { texRight,texBottom };

	vertexData_[RT].position = { right,top,0.0f,1.0f };
	vertexData_[RT].texcoord = { texRight,texTop };


	SetMaterialData(color);

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
}

void Sprite::Update()
{
	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		worldTransform[index].UpdateWorldMatrix();
		Matrix4x4 worldMatrix = MakeAffinMatrix(worldTransform[index].scale_, worldTransform[index].rotation_, worldTransform[index].translation_);
		instancingData[index] = worldMatrix;
	}

	
	UpdateVertexBuffer();

#ifdef _DEBUG
	ImGui::Begin("uvTransform");
	ImGui::DragFloat2("UVTransform", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransform.rotate.z);
	ImGui::End();
#endif // _DEBUG


	Matrix4x4 uvTransformMatrix_ = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranselateMatrix(uvTransform.translate));
	materialData_->uvTransform = uvTransformMatrix_;



}

void Sprite::Draw(ICamera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//ind
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetGPUHandle(textureHandle_));

	////wvp用のCbufferの場所を設定
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer_->GetGPUVirtualAddress());
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);


	//描画
	/*dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);*/
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, kNumInstance, 0, 0, 0);
}

void Sprite::SetVertexData(const float left, const float right, const float top, const float bottom)
{
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}

void Sprite::SetMaterialData(const Vector4 color)
{
	materialData_[0].color = color;
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
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->uvTransform = MakeIdentity4x4();

}

void Sprite::IndexBuffer()
{
	indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 6);
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
}

void Sprite::UpdateVertexBuffer()
{


	//テクスチャのサイズを合わせる
	left = (0.0f - anchorPoint_.x) * size_.x;
	right = (1.0f - anchorPoint_.x) * size_.x;
	top = (0.0f - anchorPoint_.y) * size_.y;
	bottom = (1.0f - anchorPoint_.y) * size_.y;

	texLeft = textureLeftTop.x / resourceDesc_.Width;
	texRight = (textureLeftTop.x + textureSize_.x) / resourceDesc_.Width;
	texTop = textureLeftTop.y / resourceDesc_.Height;
	texBottom = (textureLeftTop.y + textureSize_.y) / resourceDesc_.Height;



	//頂点の設定
	vertexData_[LB].position = { left,bottom,0.0f,1.0f };
	vertexData_[LB].texcoord = { texLeft,texBottom };

	vertexData_[LT].position = { left,top,0.0f,1.0f };
	vertexData_[LT].texcoord = { texLeft,texTop };

	vertexData_[RB].position = { right,bottom,0.0f,1.0f };
	vertexData_[RB].texcoord = { texRight,texBottom };

	vertexData_[RT].position = { right,top,0.0f,1.0f };
	vertexData_[RT].texcoord = { texRight,texTop };

	
}


void Sprite::AdjustTextureSize() {
	//テクスチャの情報を取得
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
	//テクスチャサイズの初期化
	textureSize_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };
}

void Sprite::SpriteDebug()
{
#ifdef _DEBUG
	ImGui::Begin("camera");

	float translate[3] = { worldTransform->translation_.x,worldTransform->translation_.y,worldTransform->translation_.z };
	ImGui::SliderFloat3("transform", translate, -20, 4);

	float rotation[3] = { worldTransform->rotation_.x,worldTransform->rotation_.y,worldTransform->rotation_.z };
	ImGui::SliderFloat3("rotation", rotation, -20, 4);

	worldTransform->translation_ = { translate[0],translate[1],translate[2] };
	worldTransform->rotation_ = { rotation[0],rotation[1],rotation[2] };

	worldTransform->UpdateWorldMatrix();

	ImGui::End();
#endif // _DEBUG


	
}

void Sprite::instancingBuffer()
{
	instancingResource = dxCommon_->CreateBufferResource(sizeof(Matrix4x4) * kNumInstance);
	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		instancingData[index] = MakeIdentity4x4();
	}
}

void Sprite::SetSRV()
{
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(Matrix4x4);
	instancingSrvHandleCPU = texture_->GetCPUDescriptorHandle(dxCommon_->GetSRVDescriptorHeap(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 5);
	instancingSrvHandleGPU = texture_->GetGPUDescriptorHandle(dxCommon_->GetSRVDescriptorHeap(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 5);

	//先頭はImGuiが使っているので次のを使う
	instancingSrvHandleCPU.ptr += (dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)*2);
	instancingSrvHandleGPU.ptr += (dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)*2);


	dxCommon_->GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}


