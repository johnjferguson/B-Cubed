Texture2D simpleTexture;
SamplerState simpleSampler;

cbuffer CBuf
{
	float4 color2;
};

float4 main(float2 tex : TEXCOORD) : SV_TARGET
{
	return simpleTexture.Sample(simpleSampler, tex);
}