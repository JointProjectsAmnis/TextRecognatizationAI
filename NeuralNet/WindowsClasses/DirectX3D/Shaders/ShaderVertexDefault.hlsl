struct Input
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct Output
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

Output main(Input input)
{
    Output output;

    output.pos = input.pos;
    output.color = input.color;

    return output;
}