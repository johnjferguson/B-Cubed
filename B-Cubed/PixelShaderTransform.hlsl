cbuffer CBuf
{
	float4 color2;
};

float4 main(float4 color : COLOR) : SV_TARGET
{
	return color;
}