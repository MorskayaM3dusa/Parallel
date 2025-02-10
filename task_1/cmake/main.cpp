#include <iostream>
#include <cmath>

// #define TYPE double
#define TYPE float
#define num 1000000

int main()
{
    TYPE arr[num];
    TYPE ans = 0;
    for(int i = 0;i<num;i++)
    {
        arr[i] = sin(M_PI * 2 * i / num);
        ans += arr[i];
    }
    std::cout << ans << std::endl;
}