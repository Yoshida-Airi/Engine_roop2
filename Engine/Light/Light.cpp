#include "Light.h"

Light::~Light()
{
	
}

void Light::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();

	lightResource_ = dxCommon_->CreateBufferResource(sizeof(PointLight));
	lightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	//pointLightData_ = new PointLight();
	pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->position = { 0.0f,2.0f,0.0f };
}

void Light::Update()
{

}

void Light::Draw(Camera* camera)
{
	//if (isInvisible_ == true)
	//{
	//	return;
	//}

	

	////index
	////dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	////形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	//dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	////マテリアルCBufferの場所を設定
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	////wvp用のCbufferの場所を設定
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform_->constBuffer_->GetGPUVirtualAddress());
	////カメラ用のCBufferの場所を設定
	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera->constBuffer_->GetGPUVirtualAddress());
	////SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	//dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetSrvGPUHandle(textureHandle_));
	////ライト用のCBufferの場所を設定
	////dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(4, lightResource_->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(5, lightResource_->GetGPUVirtualAddress());


	//
	////描画
	////dxCommon_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
	//dxCommon_->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);

}
