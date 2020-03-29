cbuffer Offset
{
	float4 offset;
	row_major matrix rotation;
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
	vso.pos = mul(pos, rotation);
	vso.pos = float4(vso.pos.x + offset.x, vso.pos.y + offset.y, offset.z, offset.w);
	//vso.pos = float4(pos.x + offset.x, pos.y + offset.y, offset.z, offset.w);

	return vso;
}