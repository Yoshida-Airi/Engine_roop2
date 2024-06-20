#include "Skybox.h"

Skybox::~Skybox()
{
	delete worldTransform_;
}

void Skybox::Initialize(uint32_t textureHandle)
{
	dxCommon_ = DirectXCommon::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	texture_ = TextureManager::GetInstance();

	VertexBuffer();
	MaterialBuffer();
	LightBuffer();
	IndexBuffer();

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	textureHandle_ = textureHandle;

	//右面。
	vertexData_[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData_[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
	//左面
	vertexData_[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	//前面
	vertexData_[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData_[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[11].position = { 1.0f,-1.0f,1.0f,1.0f };
	// 後面
	vertexData_[12].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[13].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[14].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[15].position = { 1.0f,-1.0f,-1.0f,1.0f };
	// 上面 
	vertexData_[16].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData_[17].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData_[18].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData_[19].position = { 1.0f,1.0f,1.0f,1.0f };
	// 下面
	vertexData_[20].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[21].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData_[22].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData_[23].position = { 1.0f,-1.0f,1.0f,1.0f };

	vertexData_->normal.x = 0.0f;
	vertexData_->normal.y = 0.0f;
	vertexData_->normal.z = 0.0f;

	vertexData_->texcoord = { 0.0f,0.0f };

	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 70.0f;

	//Lightingを有効にする
	materialData_->enableLighting = true;

	//ライトのデフォルト値

	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;

	IndexData();

	
}

void Skybox::Update()
{
	worldTransform_->UpdateWorldMatrix();

#ifdef _DEBUG

	ImGui::Begin("light");

	float direction[] = { lightData_->direction.x,lightData_->direction.y,lightData_->direction.z };
	ImGui::DragFloat3("lightDirection", direction, 0.1f);

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

void Skybox::Draw(Camera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}

	dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().skybox.rootSignature.Get());
	dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().skybox.graphicPipelineState.Get());

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
	//ライト用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());
	//描画
	//dxCommon_->GetCommandList()->DrawInstanced(totalVertex, 1, 0, 0);
	dxCommon_->GetCommandList()->DrawIndexedInstanced(36, 1, 0, 0, 0);
}

Skybox* Skybox::Create(uint32_t textureHandle)
{
	Skybox* skybox = new Skybox();
	skybox->Initialize(textureHandle);
	return skybox;
}

void Skybox::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * totalVertex);	//頂点用のデータ
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * totalVertex;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Skybox::MaterialBuffer()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Skybox::LightBuffer()
{
	lightResource_ = dxCommon_->CreateBufferResource(sizeof(DirectionalLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&lightData_));
}

void Skybox::IndexBuffer()
{
	indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 36);
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 36;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
}

void Skybox::IndexData()
{
	//インデックスデータ
	//右面
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 2; indexData_[4] = 1; indexData_[5] = 3;
	//左面
	indexData_[6] = 4; indexData_[7] = 5; indexData_[8] = 6;
	indexData_[9] = 6; indexData_[10] = 5; indexData_[11] = 7;
	//前面
	indexData_[12] = 8; indexData_[13] = 9; indexData_[14] = 10;
	indexData_[15] = 10; indexData_[16] = 9; indexData_[17] = 11;
	//後面
	indexData_[18] = 12; indexData_[19] = 14; indexData_[20] = 13;
	indexData_[21] = 14; indexData_[22] = 15; indexData_[23] = 13;
	//上面
	indexData_[24] = 16; indexData_[25] = 17; indexData_[26] = 18;
	indexData_[27] = 18; indexData_[28] = 17; indexData_[29] = 19;
	//下面
	indexData_[30] = 20; indexData_[31] = 22; indexData_[32] = 21;
	indexData_[33] = 22; indexData_[34] = 23; indexData_[35] = 21;
}
