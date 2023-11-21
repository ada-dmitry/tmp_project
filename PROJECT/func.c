#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double f(double x)
{
    return x + x - 16;
}

void find_zero(double *pa, double *pb, double eps)
{
    double x;
    do
    {
        x = (*pa + *pb) / 2; // метод деления отрезка пополам
        if (f(*pa) * f(x) < 0)
            *pb = x;
        else
            *pa = x;
    } while (fabs(*pb - *pa) > eps);
}

void ctrlc_handler(int signum);
void choose_path(char *choice);
void new_row(double *pa, double *pb);