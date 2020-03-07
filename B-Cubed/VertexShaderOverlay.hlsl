
struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};


VSOut main(float2 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut vso;
	vso.tex = tex;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);

	return vso;
}