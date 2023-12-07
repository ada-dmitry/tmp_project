#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "declare.h"

double a, b, eps, x, *pa, *pb, *px;

int main()
{
    double tmp;
    pa = &a;
    pb = &b;
    px = &x;

    struct sigaction sig; // Инициализация структуры, отвечающей за обработку сигнала.
    // Замена стандартного алгоритма на необходимый для вызова меню действий.
    sig.sa_handler = ctrlc_handler;
    sig.sa_flags = 0;          // Очистка флагов "на всякий случай".
    sigemptyset(&sig.sa_mask); // Очистка маски "на всякий случай".

    if (sigaction(SIGINT, &sig, NULL) == -1) // Обработчик ошибок и инициализация сигнала SIGINT.
    {
        perror("sigaction");
        return 1;
    }

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    if (*pa > *pb) // Если пользователь перепутал границы
    {
        printf("Границы указаны неверно, запуск задачи с исправленным условием\n");
        tmp = *pa;
        *pa = *pb;
        *pb = tmp;
    }

    if (f(*pa) * f(*pb) > 0)
    {
        printf("В указанных границах нуля функции не существует.\n");
        printf("Нажмите Ctrl+C и поменяйте границы\n");
    }
    if (eps < 0) // Если пользователь указал отрицательную точность
    {
        eps = eps * (-1);
        printf("Точность отрицательна, условие приведено в порядок, запуск решения...\n");
    }

    if (*pa == *pb)
    {
        if (f(*pa) == 0)
        {
            printf("\nКорень уравнения: %lf\n", *pa);
            printf("Работа программы завершена.\n");
            exit(0);
        }
        else
        {
            printf("Нажмите Ctrl+C и поменяйте границы\n");
        }
    }

    fork_method();

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}

// MEEEEEEEOW