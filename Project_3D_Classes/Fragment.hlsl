/*Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LIGHT : register(b0)
{
	float3 pos;
	float4 color;
}

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

float4 PS_main(GS_OUT input) : SV_Target
{
	// Ambient
	float4 final_colour = float4(0.0f, 0.0f, 0.0f, 1.0f) * color;

	// Diffuse shading
	float diffuse = dot(input.norm, normalize(pos - input.worldPos));
	float4 mColor = shaderTexture.Sample(SampleType, input.tex);
	float4 colour = mColor * color * max(0.0f, diffuse);
	final_colour += colour;

	return saturate(final_colour);
};*/

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
