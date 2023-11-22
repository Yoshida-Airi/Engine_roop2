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

struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
};

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float32_t4x4 ViewProjectionMatrix = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    output.position = mul(input.position, mul(gTransformationMatrix.WorldMatrix, ViewProjectionMatrix));
    return output;
}