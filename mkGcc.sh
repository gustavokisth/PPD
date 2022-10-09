cd /CMSIS_5/CMSIS/NN/Examples/ARM/arm_nn_examples/cifar10

echo "Apagando arquivos antigos ..."
rm ./Execucao* || true


echo "compilando arquivo Serial com Gcc"
gcc -O2 -g -w -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoSerial -lm

./ExecucaoSerial

echo "compilando arquivo OpenMp com Gcc"
gcc -O2 -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComOpenmp -lm

./ExecucaoComOpenmp 1
./ExecucaoComOpenmp 2
./ExecucaoComOpenmp 3
./ExecucaoComOpenmp 4

echo "compilando arquivo PThreads com Gcc"
gcc -O2 -g -w -pthread -D_PTHREAD -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComPthread  -lm

./ExecucaoComPthread 1
./ExecucaoComPthread 2
./ExecucaoComPthread 3
./ExecucaoComPthread 4