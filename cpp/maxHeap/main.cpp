#include <iostream>
#include <queue>
#include <vector>

int main(void)
{
    std::ios_base::sync_with_stdio(false);
    auto max_cnt = 0;
    std::cin >> max_cnt;

    std::priority_queue<int> max_heap;
    std::vector<int> result;
    auto x = 0;
    for (int i = 0; i < max_cnt; ++i)
    {
        std::cin >> x;
        if (x != 0)
        {
            max_heap.push(x);
        }
        else if (!max_heap.empty())
        {
            result.push_back(max_heap.top());
            max_heap.pop();
        }
        else
        {
            result.push_back(0);
        }
    }

    for (const auto &it : result)
    {
        std::cout << it << '\n';
    }

    return 0;
}