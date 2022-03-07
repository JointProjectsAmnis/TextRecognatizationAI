struct Input
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

float4 main(Input input) : SV_TARGET
{
    return input.color;
}