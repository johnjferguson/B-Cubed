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
	float4 depthPosition : DEPTHPOSITION;
	float4 pos : SV_POSITION;
};


VSOut main( float4 pos : POSITION)
{
	VSOut vso;
	pos.w = 1.0f;
	vso.pos = mul(pos, world);
	vso.pos = mul(vso.pos, view);
	vso.pos = mul(vso.pos, perspective);
	vso.depthPosition = vso.pos;
	return vso;
}