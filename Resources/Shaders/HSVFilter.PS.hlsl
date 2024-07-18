#include"FullScreen.hlsli"

struct Material
{
    float32_t hue;
    float32_t saturation;
    float32_t value;
};


ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float32_t3 RGBToHSV(float32_t3 rgb)
{
   
    float r = rgb.r;
    float g = rgb.g;
    float b = rgb.b;

    float max = r > g ? r : g;
    max = max > b ? max : b;
    float min = r < g ? r : g;
    min = min < b ? min : b;
    float h = max - min;

    if (h > 0.0f)
    {
        if (max == r)
        {
            h = (g - b) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == g)
        {
            h = 2.0f + (b - r) / h;
        }
        else
        {
            h = 4.0f + (r - g) / h;
        }
    }
    h /= 6.0f;
    float s = (max - min);
    if (max != 0.0f)
        s /= max;
    float v = max;

    return float3(h, s, v);
}

float32_t3 HSVToRGB(float32_t3 hsv)
{
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;

    float r = v;
    float g = v;
    float b = v;

    if (s > 0.0f)
    {
        h *= 6.0f;
        int i = (int) h;
        float f = h - (float) i;
        switch (i)
        {
            default:
            case 0:
                g *= 1.0f - s * (1.0f - f);
                b *= 1.0f - s;
                break;
            case 1:
                r *= 1.0f - s * f;
                b *= 1.0f - s;
                break;
            case 2:
                r *= 1.0f - s;
                b *= 1.0f - s * (1.0f - f);
                break;
            case 3:
                r *= 1.0f - s;
                g *= 1.0f - s * f;
                break;
            case 4:
                r *= 1.0f - s * (1.0f - f);
                g *= 1.0f - s;
                break;
            case 5:
                g *= 1.0f - s;
                b *= 1.0f - s * f;
                break;
        }
    }

    return float3(r, g, b);
}


float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange, range);
    if(modValue<0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

PixcelShaderOutput main(VertexShaderOutput input)
{
    PixcelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    //RGBからHSV
    float32_t3 hsv = RGBToHSV(textureColor.rgb);
    
    //各パラメータを増減
    hsv.x += gMaterial.hue;
    hsv.y += gMaterial.saturation;
    hsv.z += gMaterial.value;
    
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    
    //HSVからRGB
    float32_t3 rgb = HSVToRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    

    return output;
}


