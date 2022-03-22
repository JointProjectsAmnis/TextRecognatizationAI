RWTexture2D<float4> m1 : register(u1);
RWTexture2D<float4> m0 : register(u0);

const int matricesCount = 2;

RWTexture2D<float4> matrices[] : register(u0);

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//m0[DTid.xy] = float4(5, 5, 5, 5);
	
	matrices[0][DTid.xy] = float4(5, 5, 5, 5);
}