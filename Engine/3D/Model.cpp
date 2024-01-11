#include "Model.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

void Model::Initialize(ModelData data)
{
	dxCommon_ = DirectXCommon::GetInstance();
	texture_ = TextureManager::GetInstance();
	modelLoader_ = ModelLoader::GetInstance();
	worldTransform_.Initialize();

	modelData_ = data;
	textureHandle_ = texture_->LoadTexture(modelData_.material.textureFilePath);

	VertexBuffer();
	MaterialBuffer();
	LightBuffer();

	
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//ライトのデフォルト値
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;
}

void Model::Update()
{
	worldTransform_.UpdateWorldMatrix();

#ifdef _DEBUG

	ImGui::Begin("light");

	float direction[] = { lightData_->direction.x,lightData_->direction.y,lightData_->direction.z };
	ImGui::SliderFloat3("lightDirection", direction, -1.0f, 1.0f);

	lightData_->direction.x = direction[0];
	lightData_->direction.y = direction[1];
	lightData_->direction.z = direction[2];


	float color[] = { materialData_->color.x,materialData_->color.y,materialData_->color.z,materialData_->color.w };
	ImGui::ColorEdit4("Pick A Color", color);

	materialData_->color.x = color[0];
	materialData_->color.y = color[1];
	materialData_->color.z = color[2];
	materialData_->color.w = color[3];

	ImGui::End();
#endif // _DEBUG

}

void Model::Draw(ICamera* camera)
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
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_.constBuffer_->GetGPUVirtualAddress());
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetGPUHandle(textureHandle_));
	//ライト用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());
	//描画
	dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

std::unique_ptr< Model>  Model::Create(ModelData data)
{
	std::unique_ptr< Model> model(new Model());
	model->Initialize(data);
	return model;
}


/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/


void Model::VertexBuffer()
{
	//頂点用リソースを作る
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

/// <summary>
/// マテリアルのバッファの取得
/// </summary>
void Model::MaterialBuffer()
{

	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}


void Model::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}
