Texture2D simpleTexture;
SamplerState simpleSampler;

cbuffer CBuf
{
	float4 lightPosition;
};

float4 main(float2 tex : TEXCOORD, float4 norm : NORMAL, float4 wpos : WORLDPOSITION) : SV_TARGET
{
	float4 ray = wpos - lightPosition;
	if (dot(ray, norm) < 0.0f)
	{
		return simpleTexture.Sample(simpleSampler, tex);
	}
	else
	{
		return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

}