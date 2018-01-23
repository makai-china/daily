#include <iostream>

using namespace std;

// m * n格子矩阵, 从左上角走到右下角
// 假设只能沿着横和竖向前走，请问共有多少种走法
// 数学排列组合问题:
// 等价于从m+n中选出m个横着走，另外n步即为竖走
// 因此答案为C((m+n), m)

// 动态规划:
// 转换方程: result(m, n) = result(m-1, n) + result(m, n-1)
// 边界条件: result(0, *) = 1 且 result(*, 0) = 1

int result(int m, int n)
{
    if(m == 0 || n == 0)
    {
        return 1;
    }
    return (result(m, n-1) + result(m-1,n));
}

int main()
{
    int m = 3, n = 3;
    int ret = result(m, n);
    cout << "ret = " << ret << endl;
    return 0;
}
