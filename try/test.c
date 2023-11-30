#include <stdio.h>

void plus1(int *px)
{
    *px += 1;
}

int main()
{
    static int x = 5; 
    static int *px = &x;
    plus1(px);
    printf('%d %d\n', x, x+1);

    return 0;


}