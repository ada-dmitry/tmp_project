#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio_ext.h>

#include "declare.h"

double a, b, eps, x, *pa, *pb, *px;
short method, *mth, sig_flag;

int main()
{
    sig_flag = 0;
    method = 0;
    mth = &method;
    pa = &a;
    pb = &b;
    px = &x;

    struct sigaction sig_int; // Инициализация структуры, отвечающей за обработку сигнала.
    // Замена стандартного алгоритма на необходимый для вызова меню действий.
    sig_int.sa_handler = ctrlc_handler;
    sig_int.sa_flags = 0;          // Очистка флагов "на всякий случай".
    sigemptyset(&sig_int.sa_mask); // Очистка маски "на всякий случай".

    if (sigaction(SIGINT, &sig_int, NULL) == -1) // Обработчик ошибок и инициализация сигнала SIGINT.
    {
        perror("sigaction");
        return 1;
    }

    show_grapgh();
    input_diap();

    if (*mth == 0)
    {
        printf("Fork\n");
        fork_method();
    }
    else if (*mth == 1)
    {
        printf("Chord\n");
        chord_method();
    }
    else if (*mth == 2)
    {
        printf("Newton\n");
        newton_method();
    }
    else
    {
        printf("Fork\n");
        fork_method();
    }

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}

// MEEEEEEEOW