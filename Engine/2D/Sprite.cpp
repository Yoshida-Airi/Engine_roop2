#include "Sprite.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Sprite::~Sprite()
{
	delete worldTransform_;
}

void Sprite::Initialize(uint32_t textureHandle)
{

	dxCommon_ = DirectXCommon::GetInstance();
	texture_ = TextureManager::GetInstance();

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	textureHandle_ = textureHandle;

	
	uvTransform =
	{
		{1.0f,1.0f,1.0f,},
		{0.0f,0.0f,0.0f,},
		{0.0f,0.0f,0.0f},
	};

	VertexBuffer();
	MaterialBuffer();
	IndexBuffer();
	InstancingBuffer();

	AdjustTextureSize();
	
	SetSRV();

	left = 0.0f * textureSize_.x;
	right = 1.0f * textureSize_.x;
	top = 0.0f * textureSize_.y;
	bottom = 1.0f * textureSize_.y;

	texLeft = textureLeftTop.x / textureSize_.x;
	texRight = (textureLeftTop.x + textureSize_.x) / textureSize_.x;
	texTop = textureLeftTop.y / textureSize_.y;
	texBottom = (textureLeftTop.y + textureSize_.y) / textureSize_.y;


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

	materialData_->enableLighting = false;

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		transform[index].scale = { 0.005f,0.005f,0.005f };
		transform[index].rotate = { 0.0f,3.14f,3.14f };
		transform[index].translate = { index * 0.1f,index * 0.1f,index * 0.1f };
	}

}

void Sprite::Update()
{
	worldTransform_->UpdateWorldMatrix();
	UpdateVertexBuffer();

	for (uint32_t index = 0; index < kNumInstance; ++index)
	{
		Matrix4x4 worldMatrix = MakeAffinMatrix(transform[index].scale, transform[index].rotate, transform[index].translate);
		instancingData[index] = worldMatrix;
	}


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

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(dxCommon_->GetPRootSignature());
	dxCommon_->GetCommandList()->SetPipelineState(dxCommon_->GetPGraphicPipelineState());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//ind
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetGPUHandle(textureHandle_));
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

void Sprite::SetAlpha()
{
	
}

Sprite* Sprite::Create(uint32_t textureHandle)
{
	Sprite* sprite = new Sprite();
	sprite->Initialize(textureHandle);
	return sprite;
}

void Sprite::Debug(const char* name)
{
#ifdef _DEBUG
	ImGui::Begin(name);
	ImGui::DragFloat2("UVTransform", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransform.rotate.z);

	ImGui::End();
#endif // _DEBUG
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
	left = (0.0f - anchorPoint_.x) * cutSize_.x;
	right = (1.0f - anchorPoint_.x) * cutSize_.x;
	top = (0.0f - anchorPoint_.y) * cutSize_.y;
	bottom = (1.0f - anchorPoint_.y) * cutSize_.y;

	texLeft = textureLeftTop.x / textureSize_.x;
	texRight = (textureLeftTop.x + cutSize_.x) / textureSize_.x;
	texTop = textureLeftTop.y / textureSize_.y;
	texBottom = (textureLeftTop.y + cutSize_.y) / textureSize_.y;



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
	cutSize_ = textureSize_;

}

void Sprite::InstancingBuffer()
{
	instancingResources_ = dxCommon_->CreateBufferResource(sizeof(Matrix4x4) * kNumInstance);
	instancingResources_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

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
	instancingSrvHandleCPU = texture_->GetCPUDescriptorHandle(dxCommon_->GetSRVDescriptorHeap(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4);
	instancingSrvHandleGPU = texture_->GetGPUDescriptorHandle(dxCommon_->GetSRVDescriptorHeap(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4);

	//先頭はImGuiが使っているので次のを使う
	instancingSrvHandleCPU.ptr += (dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5);
	instancingSrvHandleGPU.ptr += (dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 5);

	dxCommon_->GetDevice()->CreateShaderResourceView(instancingResources_.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}
