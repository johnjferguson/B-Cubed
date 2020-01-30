Texture2D simpleTexture;
SamplerState simpleSampler;

float4 main(float2 tex : TEXCOORD) : SV_TARGET
{
	return simpleTexture.Sample(simpleSampler, tex);
}