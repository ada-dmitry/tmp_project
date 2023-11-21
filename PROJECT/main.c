#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

double f(double x);                                             // function x
void find_zero(double *pa, double *pb, double eps, double *px); //
void ctrlc_handler(int signum);
// void choose_path(char *choice);
// void new_row(double *pa, double *pb);

int main()
{
    double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
    char choice = ' ', *ch = &choice;
    pthread_t thread;
    sigset_t set;

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    // Создание набора сигналов и добавление в него SIGINT
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    // Блокировка сигналов в наборе для основного потока
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
    {
        perror("pthread_sigmask");
        return 1;
    }

    // Создание потока для обработчика сигнала
    if (pthread_create(&thread, NULL, ctrlc_handler, &set) != 0)
    {
        perror("pthread_create");
        return 1;
    }

    printf("Ожидание сигнала Ctrl+C...\n");

    find_zero(pa, pb, eps, px);
    while (1)
    {
        sleep(1);
    }

    signal(SIGINT, ctrlc_handler);

    printf("Корень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}
