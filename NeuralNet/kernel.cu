//
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//
//#include <stdio.h>
//#include <thread>
//
//__global__ void add(int* a, int* b, int* c)
//{
//
//}
//
//long main()
//{
//    long N = 1920 * 1080;
//    int* a = new int[N];
//    int* b = new int[N];
//    int* c = new int[N];
//
//    int* div_a;
//    int* div_b;
//    int* div_c;
//
//    cudaMalloc(&div_a, sizeof(int) * N);
//    cudaMalloc(&div_b, sizeof(int) * N);
//    cudaMalloc(&div_c, sizeof(int) * N);
//
//    for (int x = 0; x < N; x++)
//    {
//        a[x] = 2;
//        b[x] = 1;
//    }
//
//    cudaMemcpy(div_a, a, sizeof(int) * N, cudaMemcpyHostToDevice);
//    cudaMemcpy(div_b, a, sizeof(int) * N, cudaMemcpyHostToDevice);
//    cudaMemcpy(div_c, c, sizeof(int) * N, cudaMemcpyHostToDevice);
//
//
//
//
//
//    return 0;
//}