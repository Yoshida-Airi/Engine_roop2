#include"Object3D.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WorldMatrix;
};

struct ViewProjectionMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection; 
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);


struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    int isUI : ISUI0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    output.texcoord = input.texcoord;

    // 通常カメラ
    float32_t4x4 ViewProjectionMatrix = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    float32_t4x4 WorldViewProjectionMatrix = mul(gTransformationMatrix.WorldMatrix, ViewProjectionMatrix);
    output.position = mul(input.position, WorldViewProjectionMatrix);
    return output;
}