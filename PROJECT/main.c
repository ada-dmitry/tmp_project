#include <stdio.h>
#include <signal.h>

double f(double x);                                             // function x
void find_zero(double *pa, double *pb, double eps, double *px); //
void ctrlc_handler(int signum);
// void choose_path(char *choice);
// void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
    char choice = ' ', *ch = &choice;

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    // if (a > b || )
    find_zero(pa, pb, eps, px);

    printf("Корень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}
