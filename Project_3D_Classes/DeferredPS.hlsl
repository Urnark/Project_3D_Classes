Texture2D shaderTexture : register(t0);
SamplerState sampleType : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : WORLDPOS;
};

struct PixelOutputType
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
};

PixelOutputType DPS_main(PixelInputType input) : SV_TARGET
{
	PixelOutputType output;

	// Sample the color from the texture and store it for output to the render target.
	output.color = shaderTexture.Sample(sampleType, input.tex);

	// Store the normal for output to the render target.
	output.normal = float4(input.normal, 1.0f);

	output.position = input.worldPos;

	return output;
}