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
	psoManager_ = GraphicsPipelineManager::GetInstance();
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

	AdjustTextureSize();
	
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


	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	

}

void Sprite::Update()
{
	worldTransform_->UpdateWorldMatrix();
	UpdateVertexBuffer();




	Matrix4x4 uvTransformMatrix_ = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranselateMatrix(uvTransform.translate));
	materialData_->uvTransform = uvTransformMatrix_;



}

void Sprite::Draw(Camera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}


	
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().sprite.rootSignature.Get());
	dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().sprite.graphicPipelineState.Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//ind
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_->constBuffer_->GetGPUVirtualAddress());
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetSrvGPUHandle(textureHandle_));
	//描画
	/*dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);*/
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
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
	ImGui::Begin("sprite");
	if (ImGui::TreeNode(name))
	{
		if (ImGui::TreeNode("uvTransform"))
		{
			ImGui::DragFloat2("UVTransform", &uvTransform.translate.x, 0.01f);
			ImGui::DragFloat2("UVScale", &uvTransform.scale.x, 0.01f);
			ImGui::SliderAngle("UVRotate", &uvTransform.rotate.z);

			float material[4] = { materialData_->color.x,materialData_->color.y,materialData_->color.z,materialData_->color.w };
			ImGui::ColorEdit4("material", material);

			materialData_->color = { material[0],material[1],material[2],material[3] };
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("transform"))
		{
			float translate[3] = { worldTransform_->translation_.x, worldTransform_->translation_.y, worldTransform_->translation_.z };
			ImGui::DragFloat3("transform", translate, 5);
			worldTransform_->translation_ = { translate[0],translate[1],translate[2] };

			float rotate[3] = { worldTransform_->rotation_.x, worldTransform_->rotation_.y, worldTransform_->rotation_.z };
			ImGui::DragFloat3("rotate", rotate, 5);
			worldTransform_->rotation_ = { rotate[0],rotate[1],rotate[2] };

			float scale[3] = { worldTransform_->scale_.x, worldTransform_->scale_.y, worldTransform_->scale_.z };
			ImGui::DragFloat3("scale", scale, 5);
			worldTransform_->scale_ = { scale[0],scale[1],scale[2] };
		

			worldTransform_->UpdateWorldMatrix();
			ImGui::TreePop();

		}
		ImGui::TreePop();
	}
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