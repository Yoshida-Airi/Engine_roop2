#include"SkinningObject3d.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WorldMatrix;
};


struct Well
{
    float32_t4x4 skeltonSpaceMatrix;
    float32_t4x4 skeltonSpaceInverseTransposeMatrix;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
StructuredBuffer<Well> gMatrixPalette : register(t0);


struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t4 weight : WEIGHT0;
    int32_t4 index : INDEX0;
};

struct Skinned
{
    float32_t4 position;
    float32_t3 normal;
};

Skinned Skinning(VertexShaderInput input)
{
    Skinned skinned;
    
    //位置の変換
    skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeltonSpaceMatrix) * input.weight.x;
    skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeltonSpaceMatrix) * input.weight.y;
    skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeltonSpaceMatrix) * input.weight.z;
    skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeltonSpaceMatrix) * input.weight.w;
    skinned.position.w = 1.0f;
    
    //法線の変換
    skinned.normal = mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.x].skeltonSpaceInverseTransposeMatrix) * input.weight.x;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.y].skeltonSpaceInverseTransposeMatrix) * input.weight.y;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.z].skeltonSpaceInverseTransposeMatrix) * input.weight.z;
    skinned.normal += mul(input.normal, (float32_t3x3) gMatrixPalette[input.index.w].skeltonSpaceInverseTransposeMatrix) * input.weight.w;
    skinned.normal = normalize(skinned.normal); //正規化して戻してあげる

    return skinned;
}

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    Skinned skinned = Skinning(input);  //まずskinning計算を行って、Skinning後の頂点情報を手に入れる
    
    //Skinning結果を使って変換

 

    // 通常カメラ
    float32_t4x4 ViewProjectionMatrix = mul(gCamera.view, gCamera.projection);
    float32_t4x4 WorldViewProjectionMatrix = mul(gTransformationMatrix.WorldMatrix, ViewProjectionMatrix);
    output.position = mul(skinned.position, WorldViewProjectionMatrix);
    output.worldPosition = gCamera.worldPosition - (mul(skinned.position, gTransformationMatrix.WorldMatrix).xyz);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(skinned.normal, (float32_t3x3) gTransformationMatrix.WorldMatrix));
    return output;
}