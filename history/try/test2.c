#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double f(double x);
void find_zero(double *pa, double *pb, double eps, double *px);
void ctrlc_handler(int signum);
// void choose_path();
// void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
    char choice, *ch = &choice;

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);
    find_zero(pa, pb, eps, px);

    printf("Ожидание сигнала Ctrl+C...\n");
    signal(SIGINT, ctrlc_handler);
    raise(SIGINT);
    // while (1)
    //     sleep(1);

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}
