#include <stdio.h>

void computeBinary(unsigned int a, int arr[32])
{
    for (int i = 31, j = 0; i >= 0; i--)
    {
        if ((a >> i) & 1)
            arr[j] = 1;
        else
            arr[j] = 0;
        j++;
    }
}

int main()
{
    int b1[32], b2[32], i = 0, j = 0;
    float frac1, frac2, rval1 = 22.0 / 7.0;
    unsigned int *p1 = (unsigned int *)&rval1;
    float rval2;
    void *ptr = &rval2;
    unsigned int *p2 = ptr;
    *p2 = 0x40490FDB;

    printf("\nIEEE 754 (32 bit) representation of %f is : \n", rval1);
    computeBinary(*p1, b1);
    for (int k = 0; k < 32; k++)
    {
        printf("%d", b1[k]);
        if (k == 0 || k == 8)
        {
            printf(" ");
        }
    }

    printf("\nIEEE 754 (32 bit) representation of %x (%f) is : \n", 0x40490FDB, rval2);
    computeBinary(*p2, b2);
    for (int k = 0; k < 32; k++)
    {
        printf("%d", b2[k]);
        if (k == 0 || k == 8)
        {
            printf(" ");
        }
    }

    printf("\n");

    while (1)
    {
        if (b1[i] != b2[i])
            break;
        i++;
    }

    frac1 = rval1 - (int)rval1;
    frac2 = rval2 - (int)rval2;
    while (1)
    {
        frac1 = frac1 * 10;
        frac2 = frac2 * 10;
        if ((int)frac1 != (int)frac2)
            break;
        j++;
    }
    printf("The two float approximations differ in the %d binary position and %d decimal position", i + 1, j + 1);
    return 0;
}