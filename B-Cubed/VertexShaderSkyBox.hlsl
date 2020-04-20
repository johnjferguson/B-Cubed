cbuffer CBuf
{
	row_major matrix world;
	row_major matrix view;
	row_major matrix perspective;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};


VSOut main(float4 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut vso;
	vso.tex = tex;
	vso.pos = mul(pos, world);
	vso.pos = mul(vso.pos, view);
	vso.pos = mul(vso.pos, perspective);
	return vso;
}