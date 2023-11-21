#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double f(double x);
void find_zero(double *pa, double *pb, double eps, double *px);
void ctrlc_handler(int signum, char *ch);
void choose_path();
void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
    char choice, *ch = &choice;
    struct sigaction sa;
    sa.sa_handler = ctrlc_handler(, ch);
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);
    find_zero(pa, pb, eps, px);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    printf("Ожидание сигнала Ctrl+C...\n");

    while (1)
    {
        sleep(1);
    }

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}
