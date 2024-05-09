#include "Model.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

Model::~Model()
{
	delete worldTransform_;
}

void Model::Initialize(const std::string& filename)
{
	dxCommon_ = DirectXCommon::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	texture_ = TextureManager::GetInstance();
	modelLoader_ = ModelLoader::GetInstance();
	animation_ = Animation::GetInstance();

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();


	modelData_ = modelLoader_->LoadModelFile(filename);

	textureHandle_ = texture_->LoadTexture(modelData_.material.textureFilePath);


	animation = animation_->LoadAnimationFile(filename);
	skelton = animation_->CreateSkelton(modelData_.rootNode);


	VertexBuffer();
	MaterialBuffer();
	LightBuffer();
	IndexBuffer();


	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 10.0f;
	materialData_->enableLighting = true;

	//ライトのデフォルト値
	lightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	lightData_->direction = { -1.0f,-1.0f,1.0f };
	lightData_->intensity = 1.0f;

}

void Model::Update()
{

	worldTransform_->UpdateWorldMatrix();

#ifdef _DEBUG
	ImGui::Begin("light");
	if (ImGui::TreeNode("light"))
	{


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


		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG

	if (animation.isValid == true)
	{
		animationTime += 1.0f / 60.0f;

		//スケルトンに適用
		animation_->ApplyAnimation(skelton, animation, animationTime);
		animation_->Update(skelton);


		animationTime = std::fmod(animationTime, animation.duration);
		NodeAnimation& rootNodeAnimation = animation.nodeAnimations[modelData_.rootNode.name];
		Vector3 translate = animation_->CalculateValue(rootNodeAnimation.translate.Keyframes, animationTime);
		Quaternion rotate = animation_->CalculateValue(rootNodeAnimation.rotate.Keyframes, animationTime);
		Vector3 scale = animation_->CalculateValue(rootNodeAnimation.scale.Keyframes, animationTime);
		Matrix4x4 localMatrix = MakeAffinMatrix(scale, rotate, translate);

		worldTransform_->matWorld_ = Multiply(localMatrix, worldTransform_->matWorld_);
		worldTransform_->TransferMatrix();

		
	}
	else
	{
		//worldTransform_->matWorld_;
		worldTransform_->TransferMatrix();
	}

}

void Model::Draw(Camera* camera)
{
	if (isInvisible_ == true)
	{
		return;
	}


	dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().object3D.rootSignature.Get());
	dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().object3D.graphicPipelineState.Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//index
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
	//dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	dxCommon_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}


Model* Model::Create(const std::string& filename)
{
	Model* model = new Model();
	model->Initialize(filename);
	return model;
}

void Model::ModelDebug(const char* name)
{
#ifdef _DEBUG
	ImGui::Begin("model");

	if (ImGui::TreeNode(name))
	{
		float translate[3] = { worldTransform_->translation_.x, worldTransform_->translation_.y, worldTransform_->translation_.z };
		ImGui::DragFloat3("transform", translate, 0.01f);
		worldTransform_->translation_ = { translate[0],translate[1],translate[2] };

		float rotate[3] = { worldTransform_->rotation_.x, worldTransform_->rotation_.y, worldTransform_->rotation_.z };
		ImGui::DragFloat3("rotate", rotate, 0.01f);
		worldTransform_->rotation_ = { rotate[0],rotate[1],rotate[2] };

		float scale[3] = { worldTransform_->scale_.x, worldTransform_->scale_.y, worldTransform_->scale_.z };
		ImGui::DragFloat3("scale", scale, 0.01f);
		worldTransform_->scale_ = { scale[0],scale[1],scale[2] };
		ImGui::TreePop();

		//worldTransform_->UpdateWorldMatrix();

	}

	ImGui::End();
#endif // _DEBUG

}

void Model::Parent(Model* model)
{
	this->worldTransform_->parent_ = model->worldTransform_;
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

void Model::IndexBuffer()
{
	indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * UINT(modelData_.indices.size());
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
}

