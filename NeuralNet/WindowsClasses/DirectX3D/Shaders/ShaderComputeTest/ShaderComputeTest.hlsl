
RWStructuredBuffer<float> buffer;

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int id = DTid.x + DTid.y * 32;

	buffer[id] = 2;
}