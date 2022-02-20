//#pragma once
//
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//
//#include <stdio.h>
//#include <thread>
//
//const long  countThreads = 1024;
//
//__global__ void add(long* a, long* b, long* c, long N)
//{
//    __shared__ long chache[countThreads];
//
//    long threadId = threadIdx.x;
//    long tid = threadIdx.x + blockIdx.x * blockDim.x;
//
//    long temp = 0;
//    while (tid < N)
//    {
//        temp += a[tid] * b[tid];
//        tid += blockDim.x * gridDim.x;
//    }
//
//    chache[threadIdx.x] = temp;
//    __syncthreads();
//
//    long i = blockDim.x / 2;
//    while (i != 0)
//    {
//        if (threadId < i)
//            chache[threadIdx.x] += chache[threadIdx.x + i];
//        __syncthreads();
//        i /= 2;
//    }
//
//    if (threadId == 0)
//        c[blockIdx.x] = chache[0];
//
//
//
//}
//
////i = 2
////16 20 10 12
//
//long tmain()
//{
//    //Объявление переменных и выделение памяти
//    float timeOfGPU = 0;
//    float timeOfCPU = 0;
//
//    long N = 1920 * 1080 * 4;
//    //long countBlocks = (N + countThreads - 1) / countThreads;
//    long countBlocks = (N + countThreads - 1) / countThreads;
//
//    long* a = new long[N];
//    long* b = new long[N];
//    long* c = new long[countBlocks];
//    long dot = 0;
//
//    long* div_a;
//    long* div_b;
//    long* div_c;
//
//    cudaMalloc(&div_a, sizeof(long) * N);
//    cudaMalloc(&div_b, sizeof(long) * N);
//    cudaMalloc(&div_c, sizeof(long) * countBlocks);
//
//    //Инициализация переменных
//    for (long x = 0; x < N; x++)
//    {
//        a[x] = 2;
//        b[x] = 1;
//    }
//
//    cudaMemcpy(div_a, a, sizeof(long) * N, cudaMemcpyHostToDevice);
//    cudaMemcpy(div_b, b, sizeof(long) * N, cudaMemcpyHostToDevice);
//    //Расчёт
//
//    cudaEvent_t start, stop;
//    cudaEventCreate(&start);
//    cudaEventCreate(&stop);
//
//    cudaEventRecord(start, 0);
//    add << < countBlocks, countThreads >> > (div_a, div_b, div_c, N);
//    cudaEventRecord(stop, 0);
//    cudaEventSynchronize(stop);
//    cudaEventElapsedTime(&timeOfGPU, start, stop);
//
//    //Копирование данных
//    cudaMemcpy(c, div_c, sizeof(long) * countBlocks, cudaMemcpyDeviceToHost);
//
//    //Дорасчёт на процессоре
//    for (long x = 0; x < countBlocks; x++)
//        dot += c[x];
//
//    //Вывод данных
//    printf("Time of calculate on GPU is %g milliseconds\n\n", timeOfGPU);
//
//
//    long posA = N - 5;
//    long posB = N - 1;
//    for (long x = posA; x <= posB; x++)
//        printf("dot(a, b) = %d\n", dot);
//    return 0;
//}