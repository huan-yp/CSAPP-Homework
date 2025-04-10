/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/
// #pragma 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"
#include <string.h>

/*
 * Please fill in the following student struct
 */

student_t student = {
    "Zhouxiang Mao",        /* Full name */
    "213242686@seu.edu.cn", /* Email address */
};

const int block_size_1 = 32;
const int block_size_2 = 128;
// const int block_size_3 = 256;

/*
Zhouxiang Mao:
Here I attach some extra infomation:
My github account is https://github.com/huan-yp.
you could view all my code for this subject here: https://github.com/huan-yp/CSAPP-Homework.
*/

/******************************************************
 * Your different versions of the pinwheel kernel go here
 ******************************************************/
char optimized_pinwheel_descr[] = "optimized_pinwheel: Optimized implementation using blocking";
void optimized_pinwheel(int dim, pixel *src, pixel *dst)
{
    // Your code here
}

/*
 * double_block_pinwheel - The naive baseline version of pinwheel
 */
void block_pinwheel(int dim, const pixel* const restrict src,  pixel* const restrict dst)
{

    // 32 size blocking and parrelelization in 4x4 core
    // copy src to new src
    //     for(int j=0;j<block_size_1;j+=2){
    //     for(int i=0;i<block_size_1-2;i+=2){
    //         *(long long*)(dst + (block_size_1 - 1 - j) * dim + i) = *(long long*)(src + i * dim + j);
    //         *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + j);

    //         *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + i) = *(long long*)(src + i * dim + (j + 1));
    //         *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 1));

    //     }
    // }
    #pragma GCC ivdep
    for(int j=0;j<block_size_1;j+=4){
        for(int i=0;i<block_size_1 - 4;i+=4){
            // *(long long*)(dst + (block_size_1 - 1 - j) * dim + i) = *(long long*)(src + i * dim + j);
            // *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + j);
            // *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + j);
            // *(dst + (block_size_1 - 1 - j) * dim + (i + 3)) = *(src + (i + 3) * dim + j);

            // *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + i) = *(long long*)(src + i * dim + (j + 1));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 1));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 1));
            // *(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 1));

            // *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + i) = *(long long*)(src + i * dim + (j + 2));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 2));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 2));
            // *(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 2));

            // *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + i) = *(long long*)(src + i * dim + (j + 3));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 3));
            // *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 3));
            // *(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 3));

            *(long long*)(dst + (block_size_1 - 1 - j) * dim + i) = *(long long*)(src + i * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + i) = *(long long*)(src + i * dim + (j + 1));
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 1));
            
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 3)) = *(long long*)(src + (i + 3) * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 1));
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 3)) = *(long long*)(src + (i + 3) * dim + (j + 1));
            
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + i) = *(long long*)(src + i * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + i) = *(long long*)(src + i * dim + (j + 3));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 3));
            
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 3)) = *(long long*)(src + (i + 3) * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 3));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 3)) = *(long long*)(src + (i + 3) * dim + (j + 3));
        }
    }
    // for(int i=block_size_1-4;i<block_size_1;i+=2)
    // for(int j=0;j<block_size_1;j+=2){
    //         *(long long*)(dst + (block_size_1 - 1 - j) * dim + i) = *(long long*)(src + i * dim + j);
    //         *(dst + (block_size_1 - 1 - j) * dim + (i + 1)) = *(src + (i + 1) * dim + j);

    //         *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + i) = *(long long*)(src + i * dim + (j + 1));
    //         *(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 1)) = *(src + (i + 1) * dim + (j + 1));

    // }
    #pragma GCC ivdep
    for(int j=0;j<block_size_1;j+=4){
            int i = block_size_1 - 4;
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + i) = *(long long*)(src + i * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + j);
            *(long long*)(dst + (block_size_1 - 1 - j) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + j);
            *(dst + (block_size_1 - 1 - j) * dim + (i + 3)) = *(src + (i + 3) * dim + j);

            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + i) = *(long long*)(src + i * dim + (j + 1));
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 1));
            *(long long*)(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 1));
            *(dst + (block_size_1 - 1 - (j + 1)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 1));

            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + i) = *(long long*)(src + i * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 2));
            *(long long*)(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 2));
            *(dst + (block_size_1 - 1 - (j + 2)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 2));

            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + i) = *(long long*)(src + i * dim + (j + 3));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 1)) = *(long long*)(src + (i + 1) * dim + (j + 3));
            *(long long*)(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 2)) = *(long long*)(src + (i + 2) * dim + (j + 3));
            *(dst + (block_size_1 - 1 - (j + 3)) * dim + (i + 3)) = *(src + (i + 3) * dim + (j + 3));
    }
}

char double_block_pinwheel_descr[] = "double_block_pinwheel: Naive baseline implementation";
void double_block_pinwheel(int dim, pixel *src, pixel *dst)
{
    // memset(dst, 0, dim * dim * sizeof(pixel)); 0.2 CPE
    // memcpy(dst, src, dim * dim * sizeof(pixel)); // 0.2 CPE


    int i, j;
    for(i = 0; i + block_size_2 < dim; i += block_size_2){
        for(j = 0; j + block_size_2 < dim; j += block_size_2){
            for(int k = i; k < i + block_size_2; k += block_size_1)
            for(int l = j; l < j + block_size_2; l += block_size_1)
                block_pinwheel(dim, src + k * dim + l,
                    dst + (dim - l - block_size_1) * dim + k);    
        }
        for(int k = i; k < i + block_size_2; k += block_size_1)
        for(int l = j; l < dim; l += block_size_1){
            block_pinwheel(dim, src + k * dim + l, 
                dst + (dim - l - block_size_1) * dim + k);
        }
    }
    for(int k = i; k < dim; k += block_size_1)
    for(int l = 0; l < dim; l += block_size_1){
        block_pinwheel(dim, src + k * dim + l,
            dst + (dim - l - block_size_1) * dim + k);   
    }

    // for (int i = 0; i < dim; i += block_size_1){
    //     for(int j = 0; j < dim; j += block_size_1){
    //         block_pinwheel(dim, src + i * dim + j, 
    //             dst + (dim - j - block_size_1) * dim + i);
    //     }
    // }
    
}       

void single_block_pinwheel(int dim, pixel *src, pixel *dst){
    for (int i = 0; i < dim; i += block_size_1){
        for(int j = 0; j < dim; j += block_size_1){
            block_pinwheel(dim, src + i * dim + j, 
                dst + (dim - j - block_size_1) * dim + i);
        }
    }
}

void multi_thread_pinwheel_hpt(int dim, const pixel *src, pixel *dst){
    for (int i = 0; i < dim / 2; i += block_size_1){
        for(int j = 0; j < dim / 2; j += block_size_1){
            block_pinwheel(dim, src + i * dim + j, 
                dst + (dim / 2 - j - block_size_1) * dim + i);
        }
    }
}

typedef struct {
    int dim;
    pixel* src;
    pixel* dst;
} ThreadArgs;

// 线程函数
void* multi_thread_pinwheel_hpt_wrapper(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    multi_thread_pinwheel_hpt(args->dim, args->src, args->dst);
    return NULL;
}

void multi_thread_single_block_pinwheel(int dim, pixel* src, pixel* dst) {
    // printf("%lu", sizeof(pixel));
    
    if(dim >= 1024){
        int cut = dim / 2;
        pthread_t threads[4];
        ThreadArgs args[4];

        // 初始化线程参数并创建线程
        args[0] = (ThreadArgs){dim, src, dst + cut * dim};
        args[1] = (ThreadArgs){dim, src + cut, dst};
        args[2] = (ThreadArgs){dim, src + cut * dim, dst + cut * dim + cut};
        args[3] = (ThreadArgs){dim, src + cut * dim + cut, dst + cut};

        pthread_create(&threads[0], NULL, multi_thread_pinwheel_hpt_wrapper, &args[0]);
        pthread_create(&threads[1], NULL, multi_thread_pinwheel_hpt_wrapper, &args[1]);
        pthread_create(&threads[2], NULL, multi_thread_pinwheel_hpt_wrapper, &args[2]);
        pthread_create(&threads[3], NULL, multi_thread_pinwheel_hpt_wrapper, &args[3]);

        // 等待所有线程完成
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
        pthread_join(threads[3], NULL);
    }
    else{
        double_block_pinwheel(dim, src, dst);
    }
}


/*
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
void pinwheel(int dim, pixel *src, pixel *dst){
    multi_thread_single_block_pinwheel(dim, src, dst);
}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_pinwheel_functions()
{
    // add_pinwheel_function(&double_block_pinwheel, "double_block_pinwheel");
    // add_pinwheel_function(&single_block_pinwheel, "single_block_pinwheel");
    add_pinwheel_function(&multi_thread_single_block_pinwheel, "multi_thread_sigle_block_pinwheel");
    // add_pinwheel_function(&optimized_pinwheel, optimized_pinwheel_descr);
    // add_pinwheel_function(&pinwheel, pinwheel_descr);
    /* ... Register additional test functions here */
}

/***************
 * motion KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the motion function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct
{
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/
char optimized_motion_descr[] = "optimized_motion: Optimized implementation";

/*
 * multi_thread_motion - The naive baseline version of motion
 */

// char naive_motion_descr[] = "multi_thread_motion: Naive baseline implementation";
// void multi_thread_motion(int dim, pixel *src, pixel *dst)
// {
//     // 分配一段 mid 大小为 dim * dim * pixel
//     pixel *mid = (pixel *)malloc(sizeof(pixel) * dim * dim);
//     const long long mask1 = 0x0fff0fff0fffll;
//     const long long mask2 = 0x07ff07ff07ffll;
//     const long long mask3 = 0x03ff03ff03ffll;
//     const long long mask4 = 0x007f007f007fll;
//     const long long mask5 = 0x003f003f003fll;
//     const long long mask6 = 0x7fff7fff7fffll;
//     const long long mask = 0xffffffffffff;
//     for(int i=0;i<dim;i++)
//     for(int j=0;j<dim;j++){
//         long long val = *((long long*)(src + RIDX(i, j, dim))); 
//         val = ((val >> 4) & mask1) + ((val >> 5) & mask2) + ((val >> 6) & mask3) + ((val >> 9) & mask4); 
//         *(mid + RIDX(i, j, dim)) = *((pixel*)&val);
//     }
// }
    //     for(int i = 1; i < dim - 1; i++){
//         {
//             long long sum=0;
//             for(int k=-1;k<=1;k++)
//                 for(int l=0;l<=1;l++){
//                     sum += *(long long*)(mid + RIDX(i + k,  l, dim)) & mask;
//                 }
//             sum = ((sum >> 1) & mask6) + sum;
//             *(dst + RIDX(i, 0, dim)) = *((pixel*)&sum);
//         }
//         {
//             long long sum=0;
//             for(int k=-1;k<=1;k++)
//                 for(int l=-1;l<=0;l++){
//                     sum += *(long long*)(mid + RIDX(i + k, dim - 1 + l, dim))  & mask;
//                 }
//             if(i == 94){
//                 printf("sum = %lld\n", sum);
//             }
//             sum = ((sum >> 1) & mask6) + sum;
//             *(dst + RIDX(i, dim - 1, dim)) = *((pixel*)&sum);
//         }
    
//         for (int j = 1; j < dim - 1; j++){
//             long long sum=0;
//             for(int k = -1; k <= 1; k++)
//                 for(int l = -1; l <= 1; l++){
//                     sum += *(long long*)(mid + RIDX(i + k, j + l, dim)) & mask;
//                 }
//             *(dst + RIDX(i, j, dim)) = *((pixel*)&sum);
//         }
//     }
//     for(int j = 0; j < dim; j++){
//         dst[RIDX(dim - 1, j, dim)] = avg(dim, dim - 1, j, src);
//         dst[RIDX(0, j, dim)] = avg(dim, 0, j, src);
//     }

// }

void optimized_motion(int dim, pixel *src, pixel *dst)
{
    // Your code here
    unsigned *_c = (unsigned *)malloc(sizeof(int) * (dim + 2) * (dim + 2) * 3);
    memset(_c, 0, sizeof(int) * (dim + 2) * (dim + 2) * 3);
    #define r(i, j) (_c + ((i + 1) * (dim + 2) + (j + 1)) * 3)
    #define g(i, j) (_c + ((i + 1) * (dim + 2) + (j + 1)) * 3 + 1)
    #define b(i, j) (_c + ((i + 1) * (dim + 2) + (j + 1)) * 3 + 2)

    // int *r = _r + (dim + 2) + 1, *g = _g + (dim + 2) + 1, *b = _b + (dim + 2) + 1;    for(int i = 1; i < dim + 1; i++)
    for(int j = 0; j < dim; j+=4){
        int i = 0;
        *r(i, j) = (unsigned)src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red;
        *g(i, j) = (unsigned)src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green;
        *b(i, j) = (unsigned)src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue;
        *r(i, j + 1) = (unsigned)src[RIDX(i, j + 1, dim)].red + src[RIDX(i + 1, j + 1, dim)].red;
        *g(i, j + 1) =  (unsigned)src[RIDX(i, j + 1, dim)].green + src[RIDX(i + 1, j + 1, dim)].green;
        *b(i, j + 1) = (unsigned)src[RIDX(i, j + 1, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue;
        *r(i, j + 2) = (unsigned)src[RIDX(i, j + 2, dim)].red + src[RIDX(i + 1, j + 2, dim)].red;
        *g(i, j + 2) =  (unsigned)src[RIDX(i, j + 2, dim)].green + src[RIDX(i + 1, j + 2, dim)].green;
        *b(i, j + 2) = (unsigned)src[RIDX(i, j + 2, dim)].blue + src[RIDX(i + 1, j + 2, dim)].blue;
        *r(i, j + 3) = (unsigned)src[RIDX(i, j + 3, dim)].red + src[RIDX(i + 1, j + 3, dim)].red;
        *g(i, j + 3) =  (unsigned)src[RIDX(i, j + 3, dim)].green + src[RIDX(i + 1, j + 3, dim)].green;
        *b(i, j + 3) = (unsigned)src[RIDX(i, j + 3, dim)].blue + src[RIDX(i + 1, j + 3, dim)].blue;
    
    }

    for(int j = 0; j < dim; j+=4){
        int i = dim - 1;
        *r(i, j) = src[RIDX(i - 1, j, dim)].red + (unsigned)src[RIDX(i, j, dim)].red;
        *g(i, j) = src[RIDX(i - 1, j, dim)].green + (unsigned)src[RIDX(i, j, dim)].green;
        *b(i, j) = src[RIDX(i - 1, j, dim)].blue + (unsigned)src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue;
        *r(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].red + (unsigned)src[RIDX(i, j + 1, dim)].red;
        *g(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].green + (unsigned)src[RIDX(i, j + 1, dim)].green;
        *b(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].blue + (unsigned)src[RIDX(i, j + 1, dim)].blue;
        *r(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].red + (unsigned)src[RIDX(i, j + 2, dim)].red;
        *g(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].green + (unsigned)src[RIDX(i, j + 2, dim)].green;
        *b(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].blue + (unsigned)src[RIDX(i, j + 2, dim)].blue;
        *r(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].red + (unsigned)src[RIDX(i, j + 3, dim)].red;
        *g(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].green + (unsigned)src[RIDX(i, j + 3, dim)].green;
        *b(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].blue + (unsigned)src[RIDX(i, j + 3, dim)].blue;
    }
    
    // for(int i = 1; i < dim - 1; i++)
    // for(int j = 0; j < dim; j+=4){
    //     *r(i, j) += src[RIDX(i, j, dim)].red;
    //     *b(i, j) += src[RIDX(i, j, dim)].blue;
    //     *g(i, j) += src[RIDX(i, j, dim)].green;
    //     *r(i, j + 1) += src[RIDX(i, j + 1, dim)].red;
    //     *g(i, j + 1) += src[RIDX(i, j + 1, dim)].green;
    //     *b(i, j + 1) += src[RIDX(i, j + 1, dim)].blue;
    //     *r(i, j + 2) += src[RIDX(i, j + 2, dim)].red;
    //     *g(i, j + 2) += src[RIDX(i, j + 2, dim)].green;
    //     *b(i, j + 2) += src[RIDX(i, j + 2, dim)].blue;
    //     *r(i, j + 3) += src[RIDX(i, j + 3, dim)].red;
    //     *g(i, j + 3) += src[RIDX(i, j + 3, dim)].green;
    //     *b(i, j + 3) += src[RIDX(i, j + 3, dim)].blue;
    // }

    // for(int i = 1; i < dim - 1; i++)
    // for(int j = 0; j < dim; j+=4){
    //     *r(i, j) += src[RIDX(i + 1, j, dim)].red;
    //     *b(i, j) += src[RIDX(i + 1, j, dim)].blue;
    //     *g(i, j) += src[RIDX(i + 1, j, dim)].green;
    //     *r(i, j + 1) += src[RIDX(i + 1, j + 1, dim)].red;
    //     *g(i, j + 1) += src[RIDX(i + 1, j + 1, dim)].green;
    //     *b(i, j + 1) += src[RIDX(i + 1, j + 1, dim)].blue;
    //     *r(i, j + 2) += src[RIDX(i + 1, j + 2, dim)].red;
    //     *g(i, j + 2) += src[RIDX(i + 1, j + 2, dim)].green;
    //     *b(i, j + 2) += src[RIDX(i + 1, j + 2, dim)].blue;
    //     *r(i, j + 3) += src[RIDX(i + 1, j + 3, dim)].red;
    //     *g(i, j + 3) += src[RIDX(i + 1, j + 3, dim)].green;
    //     *b(i, j + 3) += src[RIDX(i + 1, j + 3, dim)].blue;
    // }

    for(int i = 1; i < dim - 1; i++)
    for(int j = 0; j < dim; j+=4){
        // *r(i, j) += src[RIDX(i - 1, j, dim)].red;
        // *b(i, j) += src[RIDX(i - 1, j, dim)].blue;
        // *g(i, j) += src[RIDX(i - 1, j, dim)].green;
        // *r(i, j + 1) += src[RIDX(i - 1, j + 1, dim)].red;
        // *g(i, j + 1) += src[RIDX(i - 1, j + 1, dim)].green;
        // *b(i, j + 1) += src[RIDX(i - 1, j + 1, dim)].blue;
        // *r(i, j + 2) += src[RIDX(i - 1, j + 2, dim)].red;
        // *g(i, j + 2) += src[RIDX(i - 1, j + 2, dim)].green;
        // *b(i, j + 2) += src[RIDX(i - 1, j + 2, dim)].blue;
        // *r(i, j + 3) += src[RIDX(i - 1, j + 3, dim)].red;
        // *g(i, j + 3) += src[RIDX(i - 1, j + 3, dim)].green;
        // *b(i, j + 3) += src[RIDX(i - 1, j + 3, dim)].blue;
        *r(i, j) = src[RIDX(i - 1, j, dim)].red + (unsigned)src[RIDX(i, j, dim)].red + src[RIDX(i + 1, j, dim)].red;
        *g(i, j) = src[RIDX(i - 1, j, dim)].green + (unsigned)src[RIDX(i, j, dim)].green + src[RIDX(i + 1, j, dim)].green;
        *b(i, j) = src[RIDX(i - 1, j, dim)].blue + (unsigned)src[RIDX(i, j, dim)].blue + src[RIDX(i + 1, j, dim)].blue;
        *r(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].red + (unsigned)src[RIDX(i, j + 1, dim)].red + src[RIDX(i + 1, j + 1, dim)].red;
        *g(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].green + (unsigned)src[RIDX(i, j + 1, dim)].green + src[RIDX(i + 1, j + 1, dim)].green;
        *b(i, j + 1) = src[RIDX(i - 1, j + 1, dim)].blue + (unsigned)src[RIDX(i, j + 1, dim)].blue + src[RIDX(i + 1, j + 1, dim)].blue;
        *r(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].red + (unsigned)src[RIDX(i, j + 2, dim)].red + src[RIDX(i + 1, j + 2, dim)].red;
        *g(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].green + (unsigned)src[RIDX(i, j + 2, dim)].green + src[RIDX(i + 1, j + 2, dim)].green;
        *b(i, j + 2) = src[RIDX(i - 1, j + 2, dim)].blue + (unsigned)src[RIDX(i, j + 2, dim)].blue + src[RIDX(i + 1, j + 2, dim)].blue;
        *r(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].red + (unsigned)src[RIDX(i, j + 3, dim)].red + src[RIDX(i + 1, j + 3, dim)].red;
        *g(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].green + (unsigned)src[RIDX(i, j + 3, dim)].green + src[RIDX(i + 1, j + 3, dim)].green;
        *b(i, j + 3) = src[RIDX(i - 1, j + 3, dim)].blue + (unsigned)src[RIDX(i, j + 3, dim)].blue + src[RIDX(i + 1, j + 3, dim)].blue;
        
        // *r(i, j + 4) = src[RIDX(i - 1, j + 4, dim)].red + (unsigned)src[RIDX(i, j + 4, dim)].red + src[RIDX(i + 1, j + 4, dim)].red;
        // *g(i, j + 4) = src[RIDX(i - 1, j + 4, dim)].green + (unsigned)src[RIDX(i, j + 4, dim)].green + src[RIDX(i + 1, j + 4, dim)].green;
        // *b(i, j + 4) = src[RIDX(i - 1, j + 4, dim)].blue + (unsigned)src[RIDX(i, j + 4, dim)].blue + src[RIDX(i + 1, j + 4, dim)].blue;
        // *r(i, j + 5) = src[RIDX(i - 1, j + 5, dim)].red + (unsigned)src[RIDX(i, j + 5, dim)].red + src[RIDX(i + 1, j + 5, dim)].red;
        // *g(i, j + 5) = src[RIDX(i - 1, j + 5, dim)].green + (unsigned)src[RIDX(i, j + 5, dim)].green + src[RIDX(i + 1, j + 5, dim)].green;
        // *b(i, j + 5) = src[RIDX(i - 1, j + 5, dim)].blue + (unsigned)src[RIDX(i, j + 5, dim)].blue + src[RIDX(i + 1, j + 5, dim)].blue;
        // *r(i, j + 6) = src[RIDX(i - 1, j + 6, dim)].red + (unsigned)src[RIDX(i, j + 6, dim)].red + src[RIDX(i + 1, j + 6, dim)].red;
        // *g(i, j + 6) = src[RIDX(i - 1, j + 6, dim)].green + (unsigned)src[RIDX(i, j + 6, dim)].green + src[RIDX(i + 1, j + 6, dim)].green;
        // *b(i, j + 6) = src[RIDX(i - 1, j + 6, dim)].blue + (unsigned)src[RIDX(i, j + 6, dim)].blue + src[RIDX(i + 1, j + 6, dim)].blue;
        // *r(i, j + 7) = src[RIDX(i - 1, j + 7, dim)].red + (unsigned)src[RIDX(i, j + 7, dim)].red + src[RIDX(i + 1, j + 7, dim)].red;
        // *g(i, j + 7) = src[RIDX(i - 1, j + 7, dim)].green + (unsigned)src[RIDX(i, j + 7, dim)].green + src[RIDX(i + 1, j + 7, dim)].green;
        // *b(i, j + 7) = src[RIDX(i - 1, j + 7, dim)].blue + (unsigned)src[RIDX(i, j + 7, dim)].blue + src[RIDX(i + 1, j + 7, dim)].blue;
    }

    for(int k = 0; k < dim; k++){
        

        for(int j = 0; j < dim; j += 4){
            unsigned sum0r = 0, sum0g = 0, sum0b = 0;
            unsigned sum1r = 0, sum1g = 0, sum1b = 0;
            unsigned sum2r = 0, sum2g = 0, sum2b = 0;
            unsigned sum3r = 0, sum3g = 0, sum3b = 0;
            #define ullp (unsigned long long *)
            sum0r = *(r(k, j - 1)) + *(r(k, j)) + *(r(k, j + 1));
            sum0g = *(g(k, j - 1)) + *(g(k, j)) + *(g(k, j + 1));
            sum0b = *(b(k, j - 1)) + *(b(k, j)) + *(b(k, j + 1));
            sum1r = *(r(k, j)) + *(r(k, j + 1)) + *(r(k, j + 2));
            sum1g = *(g(k, j)) + *(g(k, j + 1)) + *(g(k, j + 2));
            sum1b = *(b(k, j)) + *(b(k, j + 1)) + *(b(k, j + 2));
            sum2r = *(r(k, j + 1)) + *(r(k, j + 2)) + *(r(k, j + 3));
            sum2g = *(g(k, j + 1)) + *(g(k, j + 2)) + *(g(k, j + 3));
            sum2b = *(b(k, j + 1)) + *(b(k, j + 2)) + *(b(k, j + 3));
            sum3r = *(r(k, j + 2)) + *(r(k, j + 3)) + *(r(k, j + 4));
            sum3g = *(g(k, j + 2)) + *(g(k, j + 3)) + *(g(k, j + 4));
            sum3b = *(b(k, j + 2)) + *(b(k, j + 3)) + *(b(k, j + 4));
            dst[RIDX(k, j, dim)].red = sum0r / 9u;
            dst[RIDX(k, j, dim)].green = sum0g / 9u;
            dst[RIDX(k, j, dim)].blue = sum0b / 9u;
            dst[RIDX(k, j + 1, dim)].red = sum1r / 9u;
            dst[RIDX(k, j + 1, dim)].green = sum1g / 9u;
            dst[RIDX(k, j + 1, dim)].blue = sum1b / 9u;
            dst[RIDX(k, j + 2, dim)].red = sum2r / 9u;
            dst[RIDX(k, j + 2, dim)].green = sum2g / 9u;
            dst[RIDX(k, j + 2, dim)].blue = sum2b / 9u;
            dst[RIDX(k, j + 3, dim)].red = sum3r / 9u;
            dst[RIDX(k, j + 3, dim)].green = sum3g / 9u;
            dst[RIDX(k, j + 3, dim)].blue = sum3b / 9u ;
            #undef ullp
        }
        
        {
            unsigned sum0r = 0, sum0g = 0, sum0b = 0;
            sum0r = *(r(k, 0)) + *(r(k, 1));
            sum0g = *(g(k, 0)) + *(g(k, 1));
            sum0b = *(b(k, 0)) + *(b(k, 1));
            dst[RIDX(k, 0, dim)].red = sum0r / 6u;
            dst[RIDX(k, 0, dim)].green = sum0g / 6u;
            dst[RIDX(k, 0, dim)].blue = sum0b / 6u;

        }

        {
            unsigned sum0r = 0, sum0g = 0, sum0b = 0;
            sum0r = *(r(k, dim - 2)) + *(r(k, dim - 1));
            sum0g = *(g(k, dim - 2)) + *(g(k, dim - 1));
            sum0b = *(b(k, dim - 2)) + *(b(k, dim - 1));
            dst[RIDX(k, dim - 1, dim)].red = sum0r / 6u;
            dst[RIDX(k, dim - 1, dim)].green = sum0g / 6u;
            dst[RIDX(k, dim - 1, dim)].blue = sum0b / 6u;
        }
    }
    
    {
        unsigned sum0r = 0, sum0g = 0, sum0b = 0;
        unsigned sum1r = 0, sum1g = 0, sum1b = 0;
        sum0r = *(r(0, 0)) + *(r(0, 1)), sum0g = *(g(0, 0)) + *(g(0, 1)), sum0b = *(b(0, 0)) + *(b(0, 1));
        sum1r = *(r(dim - 1, 0)) + *(r(dim - 1, 1)), sum1g = *(g(dim - 1, 0)) + *(g(dim - 1, 1)), sum1b = *(b(dim - 1, 0)) + *(b(dim - 1, 1));
        dst[RIDX(0, 0, dim)].red = sum0r / 4u;
        dst[RIDX(0, 0, dim)].green = sum0g / 4u;
        dst[RIDX(0, 0, dim)].blue = sum0b / 4u;
        dst[RIDX(dim - 1, 0, dim)].red = sum1r / 4u;
        dst[RIDX(dim - 1, 0, dim)].green = sum1g / 4u;
        dst[RIDX(dim - 1, 0, dim)].blue = sum1b / 4u;
    }

    for(int j = 1; j < dim - 1; j++){
        unsigned sum0r = 0, sum0g = 0, sum0b = 0;
        unsigned sum1r = 0, sum1g = 0, sum1b = 0;
        sum0r = *(r(0, j - 1)) + *(r(0, j)) + *(r(0, j + 1));
        sum0g = *(g(0, j - 1)) + *(g(0, j)) + *(g(0, j + 1));
        sum0b = *(b(0, j - 1)) + *(b(0, j)) + *(b(0, j + 1));
        sum1r = *(r(dim - 1, j - 1)) + *(r(dim - 1, j)) + *(r(dim - 1, j + 1));
        sum1g = *(g(dim - 1, j - 1)) + *(g(dim - 1, j)) + *(g(dim - 1, j + 1));
        sum1b = *(b(dim - 1, j - 1)) + *(b(dim - 1, j)) + *(b(dim - 1, j + 1));
        dst[RIDX(0, j, dim)].red = sum0r / 6u;
        dst[RIDX(0, j, dim)].green = sum0g / 6u;
        dst[RIDX(0, j, dim)].blue = sum0b / 6u;
        dst[RIDX(dim - 1, j, dim)].red = sum1r / 6u;
        dst[RIDX(dim - 1, j, dim)].green = sum1g / 6u;
        dst[RIDX(dim - 1, j, dim)].blue = sum1b / 6u;

    }

    {
        unsigned sum0r = 0, sum0g = 0, sum0b = 0;
        unsigned sum1r = 0, sum1g = 0, sum1b = 0;
        sum0r = *(r(0, dim - 2)) + *(r(0, dim - 1)), sum0g = *(g(0, dim - 2)) + *(g(0, dim - 1)), sum0b = *(b(0, dim - 2)) + *(b(0, dim - 1));
        sum1r = *(r(dim - 1, dim - 2)) + *(r(dim - 1, dim - 1)), sum1g = *(g(dim - 1, dim - 2)) + *(g(dim - 1, dim - 1)), sum1b = *(b(dim - 1, dim - 2)) + *(b(dim - 1, dim - 1));
        dst[RIDX(0, dim - 1, dim)].red = sum0r / 4u;
        dst[RIDX(0, dim - 1, dim)].green = sum0g / 4u;
        dst[RIDX(0, dim - 1, dim)].blue = sum0b / 4u;
        dst[RIDX(dim - 1, dim - 1, dim)].red = sum1r / 4u;
        dst[RIDX(dim - 1, dim - 1, dim)].green = sum1g / 4u;
        dst[RIDX(dim - 1, dim - 1, dim)].blue = sum1b / 4u;
    }

    free(_c);

}

/*
 * motion - Your current working version of motion.
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst)
{
    // multi_thread_motion(dim, src, dst);
    optimized_motion(dim, src, dst);
}

/*********************************************************************
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_motion_functions()
{
    add_motion_function(&motion, motion_descr);
    // add_motion_function(&multi_thread_motion, naive_motion_descr);
    // add_motion_function(&optimized_motion, optimized_motion_descr);
    /* ... Register additional test functions here */
}
