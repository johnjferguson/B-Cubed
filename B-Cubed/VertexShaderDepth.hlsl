cbuffer CBuf
{
	row_major matrix transform;
	row_major matrix perspective;
	row_major matrix rollpitchyaw;
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
	vso.pos = mul(pos, transform);
	vso.pos = mul(vso.pos, perspective);
	vso.depthPosition = vso.pos;
	return vso;
}