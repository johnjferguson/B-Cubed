cbuffer CBuf
{
	row_major matrix transform;
	row_major matrix perspective;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	pos = mul(pos, transform);
	pos = mul(pos, perspective);
	
	return pos;
}