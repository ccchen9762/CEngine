struct VS_INPUT {
	float4 position: SV_Position;
	float4 color: COLOR0;
};

struct VS_OUTPUT {
	float4 position: SV_Position;
	float4 color: COLOR0;
};

// shader expect column major matrix
cbuffer CBuf {
	//row_major matrix transform;
	matrix transform;
};

/*struct VS_OUTPUT
{
	float4 Position   : POSITION;
	float3 Diffuse    : COLOR0;
	float3 Specular   : COLOR1;
	float3 HalfVector : TEXCOORD3;
	float3 Fresnel    : TEXCOORD2;
	float3 Reflection : TEXCOORD0;
	float3 NoiseCoord : TEXCOORD1;
};*/


VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.position = mul(input.position, transform);
	output.color = input.color;

	return output;
}