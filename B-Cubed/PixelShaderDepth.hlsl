
float4 main(float4 depthPosition : WORLDPOSITION) : SV_TARGET
{
	float depthValue = depthPosition.z / depthPosition.w;

	return float4(depthValue, depthValue, depthValue, 1.0f);
}