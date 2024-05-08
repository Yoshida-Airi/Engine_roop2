#include"CopyImage.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixcelShaderOutput
{
	float32_t4 color : SV_TARGET0;
};

PixcelShaderOutput main(VertexShaderOutput input)
{
	PixcelShaderOutput output;
	output.color = gTexture.Sample(gSampler, input.texcoord);
	return output;
}

