Texture2D tex;
SamplerState sm;

cbuffer buffer
{
	float4 size;
	float4 backgroundColor;
};

struct Input
{
	float4 pos : SV_POSITION;
	float4 pixelPos : POSITION;
};

bool Clamp(float2 pos, float4 rect)
{
	return (pos.x >= rect.x && pos.x <= rect.z) && (pos.y >= rect.y && pos.y <= rect.w);
}

float4 main(Input input) : SV_TARGET
{
	float cof = (float)(size[3] * size[0]) / (size[1] * size[2]);

	float  spriteOffsetY = (1 - 1.0f / cof) / 2.0f;

	float4 spriteRect = float4(0, 0, 1, 1.0f / cof);

	float2 screenPos = float2((input.pixelPos.x + 1) / 2.0f, 1 - ((input.pixelPos.y + 1) / 2.0f));

	float2 spritePos = {0, (1 - 1.0f / cof) / 2.0f};

	if (Clamp(screenPos - spritePos, spriteRect))
	{
		float2 coord = float2(screenPos.x, (screenPos.y - spritePos.y) * cof);
		float4 imagePixel = tex.Sample(sm, coord);
		return float4(imagePixel.x, imagePixel.y, imagePixel.z, 1);
	}
	else
	{
		return backgroundColor;
	}
}