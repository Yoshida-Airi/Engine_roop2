#include"Particle.hlsli"

struct ParticleForGPU
{
    float32_t4x4 WorldMatrix;
    float32_t4 color;
};

struct ViewProjectionMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
};

StructuredBuffer<ParticleForGPU> gParticle : register(t0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);


struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    output.texcoord = input.texcoord;

    // 通常カメラ
    float32_t4x4 ViewProjectionMatrix = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    float32_t4x4 WorldViewProjectionMatrix = mul(gParticle[instanceId].WorldMatrix, ViewProjectionMatrix);
    output.position = mul(input.position, WorldViewProjectionMatrix);
    output.color = gParticle[instanceId].color;
    
    return output;
}