#include"PointLight.hlsli"

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

struct PointLight
{
    float32_t4 color;
    float32_t3 position;
    float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b2);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    
    //並行光源の設定
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float32_t3 toEye = normalize(input.worldPosition);
    float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    
    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    float NDotH = dot(normalize(input.normal), halfVector);
    float RdotE = dot(reflectLight, toEye);
    float specularPow = pow(saturate(NDotH), gMaterial.shininess);
    
    
    //ポイントライトの設定
    float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
    
    float32_t3 PreflectLight = reflect(pointLightDirection, normalize(input.normal));
    float PNDotH = dot(normalize(input.normal), -pointLightDirection);
    float PRdotE = dot(PreflectLight, toEye);
    float PspecularPow = pow(saturate(PNDotH), gMaterial.shininess);
    float PNdotL = saturate(dot(normalize(input.normal), -pointLightDirection));
    
   // float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    //float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
   // float NDotH = dot(normalize(input.normal), halfVector);
   // float RdotE = dot(reflectLight, toEye);
    //float specularPow = pow(saturate(NDotH), gMaterial.shininess);
    
    
    if (gMaterial.enableLighting != 0)
    {
        
        float NdotL = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float Pcos = pow(PNdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 diffuseDirectionalLight = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float32_t3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * Pcos * gPointLight.intensity;
     
       
        // 鏡面反射
        float32_t3 specularDirectinalLight = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        float32_t3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity * PspecularPow * float32_t3(1.0f, 1.0f, 1.0f);
   
        
        // すべて加算
        output.color.rgb = diffuseDirectionalLight + specularDirectinalLight + diffusePointLight + specularPointLight;
        output.color.a = gMaterial.color.a * textureColor.a;
        
        if (output.color.a == 0.0)
        {
            discard;
        }
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