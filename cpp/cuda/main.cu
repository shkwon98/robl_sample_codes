#include <vector>

// __global__ 키워드를 붙이면 device에서 작동된다.
__global__ void add(int n, float *a, float *b, float *c)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    int N = 300000000;
    std::vector<float> a(N, 1.0f);
    std::vector<float> b(N, 1.0f);
    std::vector<float> c(N, 1.0f);

    float *dev_a, *dev_b, *dev_c;
    cudaMalloc(&dev_a, N * sizeof(float));
    cudaMalloc(&dev_b, N * sizeof(float));
    cudaMalloc(&dev_c, N * sizeof(float));

    cudaMemcpy(dev_a, a.data(), N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b.data(), N * sizeof(float), cudaMemcpyHostToDevice);

    add<<<(N + 255) / 256, 256>>>(N, dev_a, dev_b, dev_c);

    cudaMemcpy(c.data(), dev_c, N * sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}