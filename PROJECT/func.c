#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>

static wait = 1;

double f(double x)
{
    return x + x - 16;
}

void find_zero(double *pa, double *pb, double eps, double *px)
{
    *px = (*pa + *pb) / 2; // метод деления отрезка пополам
    if (f(*pa) * f(*px) < 0)
        *pb = *px;
    else
        *pa = *px;
    printf("Текущее приближение: %lf\n", *px);
}

void ctrlc_handler(int signum)
{

    printf("\nПолучен сигнал Ctrl+C\n");
    printf("Продолжить поиск корня? (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): ");
    while (getchar() != '\n')
    {
        printf("listener: stop");
        wait = 0;
        _getch();
    }
}
// void choose_path(char *choice);
// void new_row(double *pa, double *pb);