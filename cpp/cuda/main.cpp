#include <vector>

int main()
{
    int N = 300000000;
    std::vector<float> a(N, 1.0f);
    std::vector<float> b(N, 1.0f);
    std::vector<float> c(N, 1.0f);

    for (int i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
    }

    return 0;
}