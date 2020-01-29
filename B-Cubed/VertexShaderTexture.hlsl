cbuffer CBuf
{
	row_major matrix transform;
	row_major matrix perspective;
	row_major matrix rollpitchyaw;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 norm : NORMAL;
	float4 wpos : WORLDPOSITION;
	float4 pos : SV_POSITION;
};


VSOut main( float4 pos : POSITION, float4 norm : NORMAL, float2 tex : TEXCOORD )
{
	VSOut vso;
	vso.tex = tex;
	vso.norm = mul(norm, rollpitchyaw);
	vso.wpos = pos;
	vso.pos = mul(pos,mul(transform, perspective));
	return vso;
}