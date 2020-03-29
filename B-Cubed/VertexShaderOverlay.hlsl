cbuffer Offset
{
	float4 offset;
};


struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};

VSOut main(float2 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut vso;
	vso.tex = tex;
	vso.pos = float4(pos.x + offset.x, pos.y + offset.y, offset.z, offset.w);

	return vso;
}