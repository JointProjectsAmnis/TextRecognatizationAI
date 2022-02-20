//#pragma once
////
////#include "cuda_runtime.h"
////#include "device_launch_parameters.h"
////
////#include <stdio.h>
////#include <thread>
////
////__global__ void add(int* a, int* b, int* c)
////{
////
////}
////
////long main()
////{
////    long N = 1920 * 1080;
////    int* a = new int[N];
////    int* b = new int[N];
////    int* c = new int[N];
////
////    int* div_a;
////    int* div_b;
////    int* div_c;
////
////    cudaMalloc(&div_a, sizeof(int) * N);
////    cudaMalloc(&div_b, sizeof(int) * N);
////    cudaMalloc(&div_c, sizeof(int) * N);
////
////    for (int x = 0; x < N; x++)
////    {
////        a[x] = 2;
////        b[x] = 1;
////    }
////
////    cudaMemcpy(div_a, a, sizeof(int) * N, cudaMemcpyHostToDevice);
////    cudaMemcpy(div_b, a, sizeof(int) * N, cudaMemcpyHostToDevice);
////    cudaMemcpy(div_c, c, sizeof(int) * N, cudaMemcpyHostToDevice);
////
////
////
////
////
////    return 0;
////}
//
//#pragma once
//
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//
//#include <stdio.h>
//#include <thread>
//
//const int countThreads = 16;
//texture<float> tex_image;
//
//__global__ void add(float* buffer)
//{
//    int x = threadIdx.x + blockIdx.x * blockDim.x;
//    int y = threadIdx.y + blockIdx.y * blockDim.y;
//    int offset = x + y * gridDim.x * blockDim.x;
//
//    buffer[offset] = tex1Dfetch(tex_image, offset) + 2;
//}
//
//void main()
//{
//    //Объявление переменных
//    int sizeX = 20;
//    int sizeY = 20;
//    int countBlocksX = (sizeX + countThreads - 1) / countThreads;
//    int countBlocksY = (sizeY + countThreads - 1) / countThreads;
//    float* image;
//
//    float* dev_image;
//    float* dev_outImage;
//
//    dim3 blocks(countBlocksX, countBlocksY);
//    dim3 threads(countThreads, countThreads);
//
//    //Инициализация переменных
//    image = new float[sizeX * sizeY];
//    cudaMalloc(&dev_image, sizeof(float) * sizeX * sizeY);
//    cudaMalloc(&dev_outImage, sizeof(float) * sizeX * sizeY);
//    cudaBindTexture(NULL, tex_image, dev_image, sizeof(float) * sizeX * sizeY);
//
//    for (int x = 0; x < sizeX * sizeY; x++)
//        image[x] = x % sizeX;
//
//    cudaMemcpy(dev_image, image, sizeof(float) * sizeX * sizeY, cudaMemcpyHostToDevice);
//
//    //Выполнение программы
//    add << <blocks, threads >> > (dev_outImage);
//    cudaMemcpy(image, dev_outImage, sizeof(float) * sizeX * sizeY, cudaMemcpyDeviceToHost);
//
//    int id = 0;
//    for (int y = 0; y < sizeY; y++)
//    {
//        for (int x = 0; x < sizeX; x++)
//        {
//            printf("%d ", (int)(image[id]));
//            id++;
//        }
//        printf("\n");
//    }
//}