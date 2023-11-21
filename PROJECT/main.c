#include <stdio.h>
#include <signal.h>

double f(double x); // function x
void find_zero(double *pa, double *pb, double eps); //
void ctrlc_handler(int signum);
void choose_path(char *choice);
void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, *pa, *pb;
    char choice;

    signal(SIGINT, ctrlc_handler);

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    // if (a > b || )

    find_zero(pa, pb, eps);

    return 0;
}
