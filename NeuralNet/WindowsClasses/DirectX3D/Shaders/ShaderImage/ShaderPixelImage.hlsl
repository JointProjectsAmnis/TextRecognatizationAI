Texture2D tex;
SamplerState sm;

cbuffer buffer
{
	float4 size;
};

struct Input
{
	float4 pos : SV_POSITION;
	float4 pixelPos : POSITION;
};

float4 main(Input input) : SV_TARGET
{
	float cof = (float)(size[3] * size[0]) / (size[1] * size[2]);

	float2 p = float2((input.pixelPos.x + 1) / 2.0f, (-(input.pixelPos.y + 1) / 2.0f) * cof);
	float4 c = tex.Sample(sm, p);
	return float4(c.x, c.y, c.z, 1);
}