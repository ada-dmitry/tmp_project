#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double f(double x)
{
    return x * x - 16;
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
    printf("\nПродолжить поиск корня? (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): \n");
}

// void choose_path()
// {
//     char choice = getchar();
//     if(choice == 'A')
//         break;
//     else if(choice == 'C')
//         find_zero()

// }
// void new_row(double *pa, double *pb);