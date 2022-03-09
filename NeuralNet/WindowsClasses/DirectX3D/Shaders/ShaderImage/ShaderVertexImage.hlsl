
struct Input
{
	float4 pixelPos : POSITION;
};

struct Output
{
	float4 pos : SV_POSITION;
	float4 pixelPos : POSITION;
};

Output main(Input input)
{
	Output output;
	output.pos = input.pixelPos;
	output.pixelPos = input.pixelPos;

	return output;
}