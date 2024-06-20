#include"Skybox.hlsli"

struct Material
{
    float32_t4 color;
    float32_t4x4 uvTransform;
    float32_t shininess;
    int32_t enableLighting;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
TextureCube<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
   // float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    //float32_t3 toEye = normalize(input.worldPosition);
    //float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    
   
   // float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    //float NDotH = dot(normalize(input.normal), halfVector);
    //float RdotE = dot(reflectLight, toEye);
    //float specularPow = pow(saturate(NDotH), gMaterial.shininess);
    
  
   
    output.color = textureColor * gMaterial.color;
     

    return output;
}