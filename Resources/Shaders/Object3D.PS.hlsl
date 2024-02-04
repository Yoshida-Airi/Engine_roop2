#include"Object3D.hlsli"

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
    int32_t enableLighting;
    float32_t shininess;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
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
    float32_t3 toEye = normalize(input.worldPosition);
    float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    
    float RdotE = dot(reflectLight, toEye);
    float specularPow = pow(max(0.0, RdotE), gMaterial.shininess);
    
    float32_t3 ks = { 1.0f, 1.0f, 1.0f };
    
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
     
        // 鏡面反射
        float32_t3 specular = gDirectionalLight.color.rgb * (ks * specularPow)*0.5f;
        // すべて加算
        output.color.rgb =specular;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
        if (output.color.a == 0.0)
        {
            discard;
        }
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    return output;
}
