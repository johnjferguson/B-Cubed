cbuffer CBuf
{
	row_major matrix world;
	row_major matrix view;
	row_major matrix perspective;
	row_major matrix lightView;
	row_major matrix lightProjection;
	float4 lightPosition;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 norm : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	float4 pos : SV_POSITION;
	float4 worldPos : WORLDPOS;
};


VSOut main( float4 pos : POSITION, float4 norm : NORMAL, float2 tex : TEXCOORD )
{
	VSOut vso;
	float4 worldPosition;

	pos.w = 1.0f;

	vso.tex = tex;

	vso.norm = mul(norm, world);
	vso.norm = normalize(vso.norm);

	vso.pos = mul(pos, world);
	vso.pos = mul(vso.pos, view);
	vso.pos = mul(vso.pos, perspective);

	vso.lightViewPosition = mul(pos, world);
	vso.lightViewPosition = mul(vso.lightViewPosition, lightView);
	vso.lightViewPosition = mul(vso.lightViewPosition, lightProjection);

	norm.w = 0.0f;
	vso.norm = mul(norm, world);
	vso.norm = normalize(vso.norm);

	worldPosition = mul(pos, world);
	vso.worldPos = worldPosition;

	vso.lightPos = lightPosition.xyz - worldPosition.xyz;

	vso.lightPos = normalize(vso.lightPos);
	
	return vso;
}