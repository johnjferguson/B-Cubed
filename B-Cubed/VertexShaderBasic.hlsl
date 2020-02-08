cbuffer CBuf
{
	row_major matrix world;
	row_major matrix view;
	row_major matrix perspective;
};

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4 output;
	output = mul(pos, world);
	output = mul(output, view);
	output = mul(output, perspective);
	
	return output;
}