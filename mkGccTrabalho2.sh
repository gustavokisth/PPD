echo "compilando arquivo OpenMp com Gcc -O0"
gcc -O0 -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComOpenmp -lm

./ExecucaoComOpenmp 1
./ExecucaoComOpenmp 2
./ExecucaoComOpenmp 3
./ExecucaoComOpenmp 4

echo "compilando arquivo OpenMp com Gcc -O3"
gcc -O3 -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComOpenmp -lm

./ExecucaoComOpenmp 1
./ExecucaoComOpenmp 2
./ExecucaoComOpenmp 3
./ExecucaoComOpenmp 4
./ExecucaoComOpenmp 0

echo "compilando arquivo OpenMp com Gcc -Os"
gcc -Os -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComOpenmp -lm

./ExecucaoComOpenmp 1
./ExecucaoComOpenmp 2
./ExecucaoComOpenmp 3
./ExecucaoComOpenmp 4

echo "compilando arquivo MPI com Gcc -O0"
mpicc -O0 -D_MPI -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComMPI -lm

mpiexec -n 1 ./ExecucaoComMPI
mpiexec -n 2 ./ExecucaoComMPI
mpiexec -n 3 ./ExecucaoComMPI
mpiexec -n 4 ./ExecucaoComMPI

echo "compilando arquivo MPI com Gcc -O3"
mpicc -O3 -D_MPI -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComMPI -lm

mpiexec -n 1 ./ExecucaoComMPI
mpiexec -n 2 ./ExecucaoComMPI
mpiexec -n 3 ./ExecucaoComMPI
mpiexec -n 4 ./ExecucaoComMPI

echo "compilando arquivo MPI com Gcc -Os"
mpicc -Os -D_MPI -g -w -fopenmp -I/CMSIS_5/CMSIS/DSP/Include -I/CMSIS_5/CMSIS/NN/Include -I/CMSIS_5/CMSIS/Core/Include /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c /CMSIS_5/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c /CMSIS_5/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c /CMSIS_5/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c /CMSIS_5/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c  /CMSIS_5/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c arm_nnexamples_cifar10.c -o ExecucaoComMPI -lm

mpiexec -n 1 ./ExecucaoComMPI
mpiexec -n 2 ./ExecucaoComMPI
mpiexec -n 3 ./ExecucaoComMPI
mpiexec -n 4 ./ExecucaoComMPI