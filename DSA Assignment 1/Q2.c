#include <stdio.h>
int main()
{
    float f = 1.0;
    int i = 0;
    while (f != 0.0)
    {
        f = f / 2.0;
        i++;
    }
    printf("no of times the loop runs : %d", i);
}