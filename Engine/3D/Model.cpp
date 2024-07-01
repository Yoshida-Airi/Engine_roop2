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
	srvManager_ = SrvManager::GetInstance();

	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();


	modelData_ = modelLoader_->LoadModelFile(filename);

	textureHandle_ = texture_->LoadTexture(modelData_.material.textureFilePath);


	animation = animation_->LoadAnimationFile(filename);

	if (animation.isValid == true)
	{
		skelton = animation_->CreateSkelton(modelData_.rootNode);
		skinCluster = CreateSkinCluster(dxCommon_->GetDevice(), skelton, modelData_);
	}

	VertexBuffer();
	MaterialBuffer();
	LightBuffer();
	IndexBuffer();


	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = 10.0f;
	materialData_->enableLighting = true;
	materialData_->environmentCoefficient = 1.0f;

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

		


		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG

	if (animation.isValid == true)
	{
		if (isActiveAnimation == true)
		{
			animationTime += 1.0f / 60.0f;
			animationTime = std::fmod(animationTime, animation.duration);
		}

		//スケルトンに適用
		animation_->ApplyAnimation(skelton, animation, animationTime);
		animation_->Update(skelton);

		ClasterUpdate(skinCluster, skelton);


		worldTransform_->TransferMatrix();
		worldTransform_->UpdateWorldMatrix();

		
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

	if (animation.isValid == true)
	{
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] =
		{
			vertexBufferView_,
			skinCluster.influenceBufferView
		};


		dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().skinningObject3D.rootSignature.Get());
		dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().skinningObject3D.graphicPipelineState.Get());

		//VBVを設定
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
	}

	if (animation.isValid == false)
	{
		dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().object3D.rootSignature.Get());
		dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().object3D.graphicPipelineState.Get());

		//VBVを設定
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	}



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
	//環境マップ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(5, texture_->GetSrvGPUHandle(environmentMapTexture));
	
	if (animation.isValid == true)
	{
		//weight用のCBufferの場所を設定
		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(6, skinCluster.paletteSrvHandle.second);
	}
	
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
		if (ImGui::TreeNode("transform"))
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
		}
		if (ImGui::TreeNode("material"))
		{
			float color[] = { materialData_->color.x,materialData_->color.y,materialData_->color.z,materialData_->color.w };
			ImGui::ColorEdit4("Pick A Color", color);

			materialData_->color.x = color[0];
			materialData_->color.y = color[1];
			materialData_->color.z = color[2];
			materialData_->color.w = color[3];

			float environmentCoefficient[]{ materialData_->environmentCoefficient };
			ImGui::SliderFloat("environmentCoefficient", environmentCoefficient, 0.0f, 1.0f);
			materialData_->environmentCoefficient = environmentCoefficient[0];

			ImGui::TreePop();
		}

		//worldTransform_->UpdateWorldMatrix();
		ImGui::TreePop();
	}

	ImGui::End();
#endif // _DEBUG

}

void Model::Parent(Model* model)
{
	this->worldTransform_->parent_ = model->worldTransform_;
}

SkinCluster Model::CreateSkinCluster(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton, const ModelData& modelData)
{
	SkinCluster skinCluster;
	uint32_t srvHandle;
	srvHandle = srvManager_->Allocate();
	

	//palette用のresourceを確保
	skinCluster.palatteResource = dxCommon_->CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.palatteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette,skeleton.joints.size() };	//spanを使ってアクセス
	skinCluster.paletteSrvHandle.first = srvManager_->GetCPUDescriptorHandle(srvHandle);
	skinCluster.paletteSrvHandle.second = srvManager_->GetGPUDescriptorHandle(srvHandle);

	//srvManager_->CreateSRVforStructuredBuffer(srvHandle, skinCluster.palatteResource.Get(), UINT(skeleton.joints.size()), WellForGPU);


	//palette用のsrvを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	device->CreateShaderResourceView(skinCluster.palatteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);


	//influence用のResourceを確保
	skinCluster.influenceResource = dxCommon_->CreateBufferResource(sizeof(VertexInfluence) * modelData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData_.vertices.size());	//0埋め。Weightを0にしておく
	skinCluster.mappedInfluence = { mappedInfluence,modelData_.vertices.size() };


	//influence用のVBVを確保
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData_.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);


	//inverseBindPoseMatrixの保存領域を作成
	skinCluster.inverseBindPoseMatrices.resize(skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4);


	//ModelDataのSkinClusterを解析してInfluenceの中身を埋める
	for (const auto& jointWeight : modelData_.skinClusterData)	
	{
		auto it = skeleton.jointmap.find(jointWeight.first);	//skeltonに対象となるjointが含まれているか判断
		if (it == skeleton.jointmap.end())
		{
			//Jointが存在しない場合、次に回す
			continue;
		}

		//該当のIndexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights)
		{
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];	//該当のvertexIndexのinfluence情報を参照
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index)
			{
				if (currentInfluence.weights[index] == 0.0f)
				{
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}

	}



	return skinCluster;

}

void Model::ClasterUpdate(SkinCluster& skinCluster, const Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex)
	{
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());

		// 行列の乗算
		Matrix4x4 skeletonSpaceMatrix = Multiply(skinCluster.inverseBindPoseMatrices[jointIndex], skeleton.joints[jointIndex].sleletonSpaceMatrix);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				assert(!isnan(skeletonSpaceMatrix.m[i][j]) && !isinf(skeletonSpaceMatrix.m[i][j]));
			}
		}

		// 逆行列と転置
		Matrix4x4 inverseMatrix = Inverse(skeletonSpaceMatrix);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				assert(!isnan(inverseMatrix.m[i][j]) && !isinf(inverseMatrix.m[i][j]));
			}
		}

		Matrix4x4 transposeMatrix = Transpose(inverseMatrix);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				assert(!isnan(transposeMatrix.m[i][j]) && !isinf(transposeMatrix.m[i][j]));
			}
		}

		// パレットに設定
		skinCluster.mappedPalette[jointIndex].skeletonSpaceMatrix = skeletonSpaceMatrix;
		skinCluster.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix = transposeMatrix;
	}


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
