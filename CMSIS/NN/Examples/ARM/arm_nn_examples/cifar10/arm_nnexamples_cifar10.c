/* ----------------------------------------------------------------------
* Copyright (C) 2010-2018 Arm Limited. All rights reserved.
*
*
* Project:       CMSIS NN Library
* Title:         arm_nnexamples_cifar10.cpp
*
* Description:   Convolutional Neural Network Example
*
* Target Processor: Cortex-M4/Cortex-M7
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*   - Neither the name of Arm LIMITED nor the names of its contributors
*     may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------- */

/**
 * @ingroup groupExamples
 */

/**
 * @defgroup CNNExample Convolutional Neural Network Example
 *
 * \par Description:
 * \par
 * Demonstrates a convolutional neural network (CNN) example with the use of convolution,
 * ReLU activation, pooling and fully-connected functions.
 *
 * \par Model definition:
 * \par
 * The CNN used in this example is based on CIFAR-10 example from Caffe [1]. 
 * The neural network consists
 * of 3 convolution layers interspersed by ReLU activation and max pooling layers, followed by a 
 * fully-connected layer at the end. The input to the network is a 32x32 pixel color image, which will 
 * be classified into one of the 10 output classes. 
 * This example model implementation needs 32.3 KB to store weights, 40 KB for activations and 
 * 3.1 KB for storing the \c im2col data.
 *
 * \image html CIFAR10_CNN.gif "Neural Network model definition"
 *
 * \par Variables Description:
 * \par
 * \li \c conv1_wt, \c conv2_wt, \c conv3_wt are convolution layer weight matrices
 * \li \c conv1_bias, \c conv2_bias, \c conv3_bias are convolution layer bias arrays
 * \li \c ip1_wt, ip1_bias point to fully-connected layer weights and biases
 * \li \c input_data points to the input image data
 * \li \c output_data points to the classification output
 * \li \c col_buffer is a buffer to store the \c im2col output
 * \li \c scratch_buffer is used to store the activation data (intermediate layer outputs)
 *
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_convolve_HWC_q7_RGB()
 * - arm_convolve_HWC_q7_fast()
 * - arm_relu_q7()
 * - arm_maxpool_q7_HWC()
 * - arm_avepool_q7_HWC()
 * - arm_fully_connected_q7_opt()
 * - arm_fully_connected_q7()
 *
 * <b> Refer  </b>
 * \link arm_nnexamples_cifar10.cpp \endlink
 *
 * \par [1] https://github.com/BVLC/caffe
 */

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "arm_math.h"
#include "arm_nnexamples_cifar10_parameter.h"
#include "arm_nnexamples_cifar10_weights.h"

#include "arm_nnfunctions.h"
#include "arm_nnexamples_cifar10_inputs.h"

#ifdef _RTE_
#include "RTE_Components.h"
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif
#endif

#if defined(_OPENMP)
#include <omp.h>
#endif
#if defined(_PTHREAD)
#include <pthread.h>
#include <semaphore.h>
int posImg = 0;
int *limInferior, *limSuperior;
#endif
#if defined(_MPI)
#include <mpi.h>
int posImg = 0;
int *limInferior, *limSuperior;
#endif

#include <exemplo_num_threads.h>
int numThreads;

// include the input and weights

static q7_t conv1_wt[CONV1_IM_CH * CONV1_KER_DIM * CONV1_KER_DIM * CONV1_OUT_CH] = CONV1_WT;
static q7_t conv1_bias[CONV1_OUT_CH] = CONV1_BIAS;

static q7_t conv2_wt[CONV2_IM_CH * CONV2_KER_DIM * CONV2_KER_DIM * CONV2_OUT_CH] = CONV2_WT;
static q7_t conv2_bias[CONV2_OUT_CH] = CONV2_BIAS;

static q7_t conv3_wt[CONV3_IM_CH * CONV3_KER_DIM * CONV3_KER_DIM * CONV3_OUT_CH] = CONV3_WT;
static q7_t conv3_bias[CONV3_OUT_CH] = CONV3_BIAS;

static q7_t ip1_wt[IP1_DIM * IP1_OUT] = IP1_WT;
static q7_t ip1_bias[IP1_OUT] = IP1_BIAS;

long int ms, ns;
time_t s;
struct timespec start, end;
int nImagens = 1000;

void vElapsedTime() {
    s  = end.tv_sec-start.tv_sec;
    if (end.tv_nsec >= start.tv_nsec) {
        ms = round(end.tv_nsec / 1.0e6)-round(start.tv_nsec / 1.0e6);
        if (ms > 999) {
            s++;
            ms = 0;
        }
        ns = end.tv_nsec-start.tv_nsec;
        ns = (ns % 1000000);
    } else {
        s--;
        ns = start.tv_nsec-end.tv_nsec;
        ns = 1.0e9 - ns;
        ms = round(ns / 1.0e6);
        ns = (ns % 1000000);
    }

    printf("*****************************************\n");
    printf("* Start time: %09ld s %09ld ns *\n", (long)start.tv_sec, start.tv_nsec);
    printf("* End   time: %09ld s %09ld ns *\n", (long)end.tv_sec, end.tv_nsec);
    printf("*****************************************\n");
    printf("* Elapsed time: %03ld s %03ld ms %06ld ns  *\n", (long)s, ms, ns);
    printf("*****************************************\n");
}

void imgFunc() {
  #ifdef RTE_Compiler_EventRecorder
  EventRecorderInitialize (EventRecordAll, 1);  // initialize and start Event Recorder
  #endif

#if defined(_OPENMP)
  omp_set_dynamic(0); //desabilita cria????o din??mica de threads, ou seja, mant??m o valor estipulado em num_threads
  #pragma omp parallel for simd num_threads(numThreads) //omp paralelo para loop for em SIMD
#endif

#if !defined(_PTHREAD) && !defined(_MPI)
    for(int x=0; x<nImagens; x++) {
#else // se ?? pthread ou MPI, tem que pegar valor para x
        int x = posImg;
#endif
    uint8_t   image_data[CONV1_IM_CH * CONV1_IM_DIM * CONV1_IM_DIM] = {0};
    q7_t      output_data[IP1_OUT];
    q7_t      col_buffer[2 * 5 * 5 * 32 * 2];
    q7_t      scratch_buffer[32 * 32 * 10 * 4];
    
    q7_t     *img_buffer1 = scratch_buffer;
    q7_t     *img_buffer2 = img_buffer1 + 32 * 32 * 32;

    memcpy(image_data, IMG_DATA_IN[x], sizeof(image_data)); //copia a imagem do input para o image_data

    /* input pre-processing */
    int mean_data[3] = INPUT_MEAN_SHIFT;
    unsigned int scale_data[3] = INPUT_RIGHT_SHIFT;
    for (int i=0;i<32*32*3; i+=3) {
      img_buffer2[i] =   (q7_t)__SSAT( ((((int)image_data[i]   - mean_data[0])<<7) + (0x1<<(scale_data[0]-1)))
                              >> scale_data[0], 8);
      img_buffer2[i+1] = (q7_t)__SSAT( ((((int)image_data[i+1] - mean_data[1])<<7) + (0x1<<(scale_data[1]-1)))
                              >> scale_data[1], 8);
      img_buffer2[i+2] = (q7_t)__SSAT( ((((int)image_data[i+2] - mean_data[2])<<7) + (0x1<<(scale_data[2]-1)))
                              >> scale_data[2], 8);
    }
    
    // conv1 img_buffer2 -> img_buffer1
    arm_convolve_HWC_q7_RGB(img_buffer2, CONV1_IM_DIM, CONV1_IM_CH, conv1_wt, CONV1_OUT_CH, CONV1_KER_DIM, CONV1_PADDING,
                            CONV1_STRIDE, conv1_bias, CONV1_BIAS_LSHIFT, CONV1_OUT_RSHIFT, img_buffer1, CONV1_OUT_DIM,
                            (q15_t *) col_buffer, NULL);

    arm_relu_q7(img_buffer1, CONV1_OUT_DIM * CONV1_OUT_DIM * CONV1_OUT_CH);

    // pool1 img_buffer1 -> img_buffer2
    arm_maxpool_q7_HWC(img_buffer1, CONV1_OUT_DIM, CONV1_OUT_CH, POOL1_KER_DIM,
                      POOL1_PADDING, POOL1_STRIDE, POOL1_OUT_DIM, NULL, img_buffer2);

    // conv2 img_buffer2 -> img_buffer1
    arm_convolve_HWC_q7_fast(img_buffer2, CONV2_IM_DIM, CONV2_IM_CH, conv2_wt, CONV2_OUT_CH, CONV2_KER_DIM,
                            CONV2_PADDING, CONV2_STRIDE, conv2_bias, CONV2_BIAS_LSHIFT, CONV2_OUT_RSHIFT, img_buffer1,
                            CONV2_OUT_DIM, (q15_t *) col_buffer, NULL);

    arm_relu_q7(img_buffer1, CONV2_OUT_DIM * CONV2_OUT_DIM * CONV2_OUT_CH);

    // pool2 img_buffer1 -> img_buffer2
    arm_maxpool_q7_HWC(img_buffer1, CONV2_OUT_DIM, CONV2_OUT_CH, POOL2_KER_DIM,
                      POOL2_PADDING, POOL2_STRIDE, POOL2_OUT_DIM, col_buffer, img_buffer2);

    // conv3 img_buffer2 -> img_buffer1
    arm_convolve_HWC_q7_fast(img_buffer2, CONV3_IM_DIM, CONV3_IM_CH, conv3_wt, CONV3_OUT_CH, CONV3_KER_DIM,
                            CONV3_PADDING, CONV3_STRIDE, conv3_bias, CONV3_BIAS_LSHIFT, CONV3_OUT_RSHIFT, img_buffer1,
                            CONV3_OUT_DIM, (q15_t *) col_buffer, NULL);

    arm_relu_q7(img_buffer1, CONV3_OUT_DIM * CONV3_OUT_DIM * CONV3_OUT_CH);

    // pool3 img_buffer-> img_buffer2
    arm_maxpool_q7_HWC(img_buffer1, CONV3_OUT_DIM, CONV3_OUT_CH, POOL3_KER_DIM,
                      POOL3_PADDING, POOL3_STRIDE, POOL3_OUT_DIM, col_buffer, img_buffer2);

    arm_fully_connected_q7_opt(img_buffer2, ip1_wt, IP1_DIM, IP1_OUT, IP1_BIAS_LSHIFT, IP1_OUT_RSHIFT, ip1_bias,
                              output_data, (q15_t *) img_buffer1);

    arm_softmax_q7(output_data, 10, output_data);

    /*for (int j = 0; j < 10; j++) {
        printf("%d= %d: %d\n", x, j, output_data[j]);
    }
    printf("\n");//*/
#if !defined(_PTHREAD) && !defined(_MPI)
    }
#endif
}

#if defined(_PTHREAD)
    void *func_pthread(void *id) {
        int idT = (int) id;

        for (int i=limInferior[idT]; i<=limSuperior[idT]; i++) {
            posImg = i;
            imgFunc();
        }
        pthread_exit(NULL);
    }
#endif

#if defined(_MPI)
    void func_mpi(int limInferior, int limSuperior) {
        for (int i=limInferior; i<=limSuperior; i++) {
            posImg = i;
            imgFunc();
        }
    }
#endif


int main(int argc, char *argv[]) {
#if defined(_MPI) //MPI
    int threadAtual;
    
    MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
    MPI_Comm_rank(MPI_COMM_WORLD, &threadAtual);

    if (threadAtual == 0) { //primeira thread a ser gerada
        printf("Executando a CNN para %d imagens ", nImagens);
        clock_gettime(CLOCK_REALTIME, &start); //tempo de in??cio da execu????o do programa
        printf("com MPI em %d threads\n", numThreads);

        limInferior = (int *) calloc(numThreads, sizeof(int)); //aloca um vetor na mem??ria de acordo com o n??mero de threads
        limSuperior = (int *) calloc(numThreads, sizeof(int)); //aloca um vetor na mem??ria de acordo com o n??mero de threads
        int total = nImagens, nThreads = numThreads;

        for (int i=0; i<numThreads; i++) { //para dividir carga de trabalho entre as threads a serem criadas
            float divide = (float) total / (float) nThreads; //divide o n??mero de imagens pelo n??mero de threads
            int arredonda = ceil(divide); //arredonda pra cima para n??o perder nenhuma imagem
            total -= arredonda; //desconta o valor do total de imagens que resta serem analisadas
            nThreads--; //desconta do valor de threads que precisam de uma carga de trabalho
            if (i == 0) { //para a primeira thread, o limite inferior sempre ?? 0 e o limite superior ?? a carga de trabalho da thread -1
                limInferior[i] = 0;
                limSuperior[i] = arredonda - 1;
                
                func_mpi(limInferior[i], limSuperior[i]);
            }
            else { //para as demais threads, o limite inferior ?? o limite superior da thread anterior +1 e o limite superior ?? o limite superior da thread anterior adicionada da carga de trabalho
                limInferior[i] = limSuperior[i-1] + 1;
                limSuperior[i] = limSuperior[i-1] + arredonda;

                MPI_Send(
                    &limInferior[i],    //dado a enviar
                    1,                  //quantidade de dados
                    MPI_INT,            //tipo de dado
                    i,                  //processo ou rank de destino
                    i,                  //identificador(tag), em inteiro
                    MPI_COMM_WORLD      //comunicador
                );
                MPI_Send(
                    &limSuperior[i],    //dado a enviar
                    1,                  //quantidade de dados
                    MPI_INT,            //tipo de dado
                    i,                  //processo ou rank de destino
                    i,                  //identificador(tag), em inteiro
                    MPI_COMM_WORLD      //comunicador
                );
            }
        }
    }
    else { //demais threads
        for (int i=1; i<numThreads; i++) {
            if (threadAtual == i) {
                MPI_Recv(
                    &limInferior,           //dado a receber
                    1,                      //quantidade de dados
                    MPI_INT,                //tipo de dado
                    0,                      //processo ou rank de origem
                    threadAtual,            //identificador(tag), em inteiro
                    MPI_COMM_WORLD,         //comunicador
                    MPI_STATUS_IGNORE       //status
                );
                MPI_Recv(
                    &limSuperior,           //dado a receber
                    1,                      //quantidade de dados
                    MPI_INT,                //tipo de dado
                    0,                      //processo ou rank de origem
                    i,                      //identificador(tag), em inteiro
                    MPI_COMM_WORLD,         //comunicador
                    MPI_STATUS_IGNORE       //status
                );

                func_mpi(limInferior, limSuperior);
            }
        }
    }

    for (int i=0; i<numThreads; i++)
        MPI_Barrier(MPI_COMM_WORLD); //espera todos os processos finalizarem antes de terminar a contagem de tempo

    if (threadAtual == 0) { //pode ser qualquer processo aqui, o importante ?? esperar todos eles finalizarem
        clock_gettime(CLOCK_REALTIME, &end); //tempo de fim da execu????o do programa
        vElapsedTime(); //chama fun????o para calcular tempo de execu????o
    }

    MPI_Finalize();

    return 0;

#else
    if (argc > 1)
        numThreads = atoi(argv[1]); //n??mero passado por par??metro na execu????o do programa
    else {
        numThreads = 2;
        puts("Voce pode escolher o numero de threads pelo comando ./exec n");
    }
    printf("Executando a CNN para %d imagens ", nImagens);
    clock_gettime(CLOCK_REALTIME, &start); //tempo de in??cio da execu????o do programa
#endif

#if defined(_OPENMP)        //OpenMP
    if (numThreads == 0) //sem limite de threads
        numThreads = omp_get_max_threads();
    printf("com OpenMP em %d threads\n", numThreads);
    imgFunc();
#elif defined(_PTHREAD)   //PThreads
    printf("com Pthreads em %d threads\n", numThreads);
    pthread_t threads[numThreads];

    limInferior = (int *) calloc(numThreads, sizeof(int)); //aloca um vetor na mem??ria de acordo com o n??mero de threads
    limSuperior = (int *) calloc(numThreads, sizeof(int)); //aloca um vetor na mem??ria de acordo com o n??mero de threads
    int total = nImagens, nThreads = numThreads;

    for (int i=0; i<numThreads; i++) { //para dividir carga de trabalho entre as threads a serem criadas
        float divide = (float) total / (float) nThreads; //divide o n??mero de imagens pelo n??mero de threads
        int arredonda = ceil(divide); //arredonda pra cima para n??o perder nenhuma imagem
        total -= arredonda; //desconta o valor do total de imagens que resta serem analisadas
        nThreads--; //desconta do valor de threads que precisam de uma carga de trabalho
        if (i == 0) { //para a primeira thread, o limite inferior sempre ?? 0 e o limite superior ?? a carga de trabalho da thread -1
            limInferior[i] = 0;
            limSuperior[i] = arredonda - 1;
        }
        else { //para as demais threads, o limite inferior ?? o limite superior da thread anterior +1 e o limite superior ?? o limite superior da thread anterior adicionada da carga de trabalho
            limInferior[i] = limSuperior[i-1] + 1;
            limSuperior[i] = limSuperior[i-1] + arredonda;
        }
        
    }

    for (int i=0; i<numThreads; i++) {
       pthread_create(&threads[i], NULL, func_pthread, (void *)i); //criando as threads
    }

    for (int i=0; i<numThreads; i++) {
        pthread_join(threads[i], NULL); //esperando as threads terminarem a execu????o
    }
#else                       //Nem OpenMP, nem Pthread, logo, ?? sequencial
    printf("de modo sequencial\n");
    imgFunc();
#endif

    clock_gettime(CLOCK_REALTIME, &end); //tempo de fim da execu????o do programa
    vElapsedTime(); //chama fun????o para calcular tempo de execu????o

    return 0;
}
