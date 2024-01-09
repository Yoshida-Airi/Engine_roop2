#include"Particle.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WorldMatrix;
};

struct ViewProjectionMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrices : register(t0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);


struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    output.texcoord = input.texcoord;

    // 通常カメラ
    float32_t4x4 ViewProjectionMatrix = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    float32_t4x4 WorldViewProjectionMatrix = mul(gTransformationMatrices[instanceId].WorldMatrix, ViewProjectionMatrix);
    output.position = mul(input.position, WorldViewProjectionMatrix);
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrices[instanceId].WorldMatrix));
    
    return output;
}