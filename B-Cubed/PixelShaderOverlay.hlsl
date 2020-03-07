Texture2D texture0: register(t0);
SamplerState sampler0;

float4 main(float2 tex : TEXCOORD) : SV_TARGET
{
	return texture0.Sample(sampler0, tex);
}