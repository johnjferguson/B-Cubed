Texture2D shaderTexture: register(t0);
Texture2D depthMapTexture: register(t1);
SamplerState SampleTypeClamp;

cbuffer CBuf
{
	float4 lightPosition;
};


float4 main(float2 tex : TEXCOORD, float4 normal : NORMAL, float4 lightViewPosition : TEXCOORD1, float3 lightPos : TEXCOORD2) : SV_TARGET
{

	float bias;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;

	bias = 0.001f;

	color = float4(0.4f,0.4f,0.4f,1.0f);

	projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		lightDepthValue = lightViewPosition.z / lightViewPosition.w;

		lightDepthValue = lightDepthValue - bias;

		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot((float3)normal, lightPos));

			if (lightIntensity > 0.0f)
			{
				color += (float4(0.2f,0.2f,0.2f,1.0f) * lightIntensity);

				color = saturate(color);
			}
		}
	}

	textureColor = shaderTexture.Sample(SampleTypeClamp, tex);

	color = color * textureColor;

	return color;

}