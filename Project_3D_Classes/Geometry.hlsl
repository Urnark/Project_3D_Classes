cbuffer BUFFER : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
}

struct GS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float3 worldPos : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN gin[3], inout TriangleStream<GS_OUT> triStream)
{
	float3 v1 = gin[1].pos.xyz - gin[0].pos.xyz;
	float3 v2 = gin[2].pos.xyz - gin[0].pos.xyz;
	float3 n = normalize(cross(v1, v2));

	const float4x4 MAT = mul(mul(world, view), proj);

	GS_OUT p = (GS_OUT)0;
	for (int i = 0; i < 3; i++)
	{
		p.pos = gin[i].pos;
		p.pos = mul(p.pos, MAT);

		// Multiply with the world matrix to make the normal rotate
		p.norm = mul(float4(n, 1), world).xyz;
		p.norm = normalize(p.norm);

		/* Multiply with the world matrix so we can calculate the 
		   direction that the light is pointing to in the FS */
		p.worldPos = mul(gin[i].pos, world).xyz;

		p.tex = gin[i].tex;

		triStream.Append(p);
	}
	triStream.RestartStrip();

	for (int i = 0; i < 3; i++)
	{
		p.pos = float4(0.8 * n + gin[i].pos.xyz, 1);
		p.pos = mul(p.pos, MAT);

		// Multiply with world matrix to make the normal rotate
		p.norm = mul(float4(n, 1), world).xyz;
		p.norm = normalize(p.norm);

		/* Multiply with the world matrix so we can calculate the
		   direction that the light is pointing to in the FS */
		p.worldPos = mul(gin[i].pos, world).xyz;

		p.tex = gin[i].tex;

		triStream.Append(p);
	}
}