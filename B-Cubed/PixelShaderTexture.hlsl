Texture2D simpleTexture;
SamplerState simpleSampler;

cbuffer CBuf
{
	float4 lightPosition;
};

float4 main(float2 tex : TEXCOORD, float4 norm : NORMAL, float4 wpos : WORLDPOSITION) : SV_TARGET
{

	float4 ray = normalize(wpos - lightPosition);
	float c = dot(ray, norm);
	if (c < 0.0f)
	{
		return simpleTexture.Sample(simpleSampler, tex);
	}
	else
	{
		return simpleTexture.Sample(simpleSampler, tex) * 0.0f;
	}

}