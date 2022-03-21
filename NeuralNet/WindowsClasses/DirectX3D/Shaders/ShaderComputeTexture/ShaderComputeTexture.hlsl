RWTexture2D<float4> outputTexture : register(u0);


[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID, uint3 DGid : SV_GroupId)
{
	int id = DTid.x + DTid.y * 1 * 8;
	outputTexture[DTid.xy] = float4(id, id + 1, id + 2, id + 3);
}