#include <vector>
#include <iostream>

// 从start到end之间找一个满足检查函数pfunc的数字添加到stack中，如果找到则添加并返回true，否则返回false。
template<typename _FuncType>
bool PushStack(std::vector<int>& stack, int start, int end, _FuncType pfunc)
{
    for (; start < end; ++start)
    {
        if (pfunc(stack, start))
        {
            stack.push_back(start);
            return true;
        }
    }

    return false;
}

// 对size个数字进行n的排列，排列需要符合pCheck函数检查，对于每一个符合的结果序列执行pFunc函数，返回排列的数量。
template<typename _CheckType, typename _FuncType>
unsigned int Permutation(unsigned int size, unsigned int n, _CheckType pCheck, _FuncType pFunc)
{
    unsigned int count = 0;
    std::vector<int> stack;

    for (unsigned int pos = 0; ;)
    {
        if (PushStack(stack, pos, size, pCheck))
        {
            if (n == stack.size())
            {
                pFunc(stack);
                ++count;

                pos = stack.back() + 1;
                stack.pop_back();
            }
            else
            {
                pos = 0;
            }
        }
        else
        {
            if (stack.empty())
                break;
            
            pos = stack.back() + 1;
            stack.pop_back();
        }
    }

    return count;
}

// 对于棋盘vtr，判定是否可以在x位置放置棋子。
bool CheckMap(const std::vector<int>& vtr, int x)
{
    for (unsigned int i = 0; i < vtr.size(); ++i)
        if (x == vtr[i] || (vtr.size() - i) == abs(x - vtr[i]))
            return false;

    return true;
}

// 打印一个盘面
void Show(const std::vector<int>& vtr)
{
    for (auto v : vtr)
    {
        for (unsigned int i = 0; i < vtr.size(); ++i)
        {
            if (v == i)
                std::cout << " *";
            else
                std::cout << " -";
        }

        std::cout << std::endl;
    }

    std::cout << "=========================" << std::endl;
}

int main()
{
    unsigned int n = Permutation(8, 8, CheckMap, Show);

    std::cout << "map count: " << n << std::endl;

    return 0;
}

