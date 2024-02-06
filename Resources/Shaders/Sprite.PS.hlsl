#include"Sprite.hlsli"

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
    int32_t enableLighting;
};


ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
    if (output.color.a == 0.0)
    {
        discard;
    }
    output.color.a = gMaterial.color.a * textureColor.a;
    
    return output;
}
