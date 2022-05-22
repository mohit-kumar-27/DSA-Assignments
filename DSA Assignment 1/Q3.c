#include <stdio.h>
int main()
{
    int n;
    printf("Enter the value of n : ");
    scanf("%d", &n);
    float sum_f = 0, sum_b = 0;
    for (int i = 1, j = n; i <= n; i++, j--)
    {
        sum_f += 1.0 / i;
        sum_b += 1.0 / j;
    }
    printf("Forward sum = %f\nBackward sum = %f", sum_f, sum_b);
    return 0;
}
