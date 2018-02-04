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

/*Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 PS_main(VS_OUT input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.tex);;
}*/

Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D positionTexture : register(t2);

SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer
{
	float3 lightPosition;
	float4 ambientColor;
	float4 diffuseColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 PS_main(PixelInputType input) : SV_TARGET
{
	float4 outputColor = ambientColor;

	// Sample the colors from the color render texture using the point sampler at this texture coordinate location.
	float4 colors = colorTexture.Sample(SampleTypePoint, input.tex);

	// Sample the normals from the normal render texture using the point sampler at this texture coordinate location.
	float4 normals = normalTexture.Sample(SampleTypePoint, input.tex);

	float4 pos = positionTexture.Sample(SampleTypePoint, input.tex);

	// Invert the light direction for calculations.
	float3 lightDir = normalize(lightPosition - pos.xyz);

	// Calculate the amount of light on this pixel.
	float lightIntensity = max(dot(normals.xyz, lightDir), 0.0f);

	// Determine the final amount of diffuse color based on the color of the pixel combined with the light intensity.
	outputColor += colors * lightIntensity * diffuseColor;

	return saturate(outputColor);
}
