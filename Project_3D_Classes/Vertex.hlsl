/*struct VS_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;//float2 tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1);
	output.tex = input.tex;

	return output;
}*/

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VS_IN
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.pos.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.pos = mul(input.pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	// Store the input color for the pixel shader to use.
	output.tex = input.tex;

	return output;
}