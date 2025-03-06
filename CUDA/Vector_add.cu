#include<iostream>
#include<array>

#define N                  100
#define NTHREADS_PER_BLOCK 256

using namespace std;


// Macro checking for errors returned by CUDA routines
#define CUDA_ERROR(routine)                               \
do {                                                      \
    const cudaError_t err = routine;                      \
    if (err != cudaSuccess) {                             \
        cout << "Routine '" << #routine << "' failed: \"" \
             << cudaGetErrorString(err) << "\"." << endl; \
    }                                                     \
} while (0)



// CUDA kernel
__global__ void arr_add_device(int *a, int *b, int *c) {
    const int i = threadIdx.x + blockIdx.x*blockDim.x;

    /* Make sure we don't try accessing memory outside the array if more than N
     * threads are launched                                                     */
    if (i < N) {
        c[i] = a[i] + b[i];
    }

    return;
}



// Main
int main() {
    // Allocate and initialize host arrays
    array<int, N> a, b, c;

    for (int i = 0; i < N; ++i) {
        a.at(i) = i;
        b.at(i) = 2*i;
        c.at(i) = 0;
    }


    // Allocate device arrays
    int *a_dev, *b_dev, *c_dev;
    const size_t size = N*sizeof(int);

    CUDA_ERROR(cudaMalloc((void**) &a_dev, size));
    CUDA_ERROR(cudaMalloc((void**) &b_dev, size));
    CUDA_ERROR(cudaMalloc((void**) &c_dev, size));


    // Copy the a and b host arrays onto the device
    // NOTE: DESTINATION first, TARGET next
    CUDA_ERROR(cudaMemcpy(a_dev, a.data(), size, cudaMemcpyHostToDevice));
    CUDA_ERROR(cudaMemcpy(b_dev, b.data(), size, cudaMemcpyHostToDevice));

    // Perform the array addition on the device
    const int nblocks = (N + NTHREADS_PER_BLOCK - 1)/NTHREADS_PER_BLOCK;
    arr_add_device<<<nblocks, NTHREADS_PER_BLOCK>>>(a_dev, b_dev, c_dev);
    CUDA_ERROR(cudaDeviceSynchronize());


    // Copy the sums array (c) back to the host and print the results
    CUDA_ERROR(cudaMemcpy(c.data(), c_dev, size, cudaMemcpyDeviceToHost));

    for (const auto &el : c) {
        cout << el << endl;
    }


    // Free up device memory
    CUDA_ERROR(cudaFree(a_dev));
    CUDA_ERROR(cudaFree(b_dev));
    CUDA_ERROR(cudaFree(c_dev));

    return 0;
}
