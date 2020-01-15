cbuffer CBuf
{
	row_major matrix transform;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};


VSOut main( float4 pos : POSITION, float2 tex : TEXCOORD )
{
	VSOut vso;
	vso.tex = tex;
	vso.pos = mul(pos,transform);
	return vso;
}