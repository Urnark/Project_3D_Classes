Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 PS_main(VS_OUT input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.tex);;
}