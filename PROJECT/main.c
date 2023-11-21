#include <stdio.h>
#include <signal.h>
#include <unistd.h>

double f(double x);                                             // function x
void find_zero(double *pa, double *pb, double eps, double *px); //
void ctrlc_handler(int signum);
// void choose_path(char *choice);
// void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
    char choice = ' ', *ch = &choice;
    sigset_t set;
    int sig;

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    // if (a > b || )

    find_zero(pa, pb, eps, px);

    // Создание набора сигналов и добавление в него SIGINT
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    // Блокировка SIGINT
    sigprocmask(SIG_BLOCK, &set, NULL);

    // Ожидание получения сигнала Ctrl+C с помощью sigwait
    sigwait(&set, &sig);
    signal(SIGINT, ctrlc_handler);

    printf("Корень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}
