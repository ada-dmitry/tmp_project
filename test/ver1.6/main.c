#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio_ext.h>
#include <termios.h>

#include "declare.h"

double a, b, eps, x, *pa, *pb, *px;
short method, *mth, sig_flag;
struct termios default_term, modif_term; // Задание структур, определяющих терминал Linux.

int main()
{
    tcgetattr(0, &default_term); // Получение текущих настроек терминала
    modif_term = default_term;
    modif_term.c_lflag &= ~(ECHO | ICANON); // Отключение символа ECHO и канонического ввода

    sig_flag = 0;
    method = 0;
    mth = &method;
    a = A;
    b = B;
    eps = EPS;
    pa = &a;
    pb = &b;
    px = &x;

    struct sigaction sig_int, sig_tstp; // Инициализация структуры, отвечающей за обработку сигнала.
    // Замена стандартного алгоритма на необходимый для вызова меню действий.
    sig_int.sa_handler = ctrlc_handler;
    sig_int.sa_flags = 0;          // Очистка флагов "на всякий случай".
    sigemptyset(&sig_int.sa_mask); // Очистка маски "на всякий случай".
    sig_tstp.sa_handler = ctrlz_handler;
    sig_tstp.sa_flags = 0;
    sigemptyset(&sig_tstp.sa_mask);

    if (sigaction(SIGINT, &sig_int, NULL) == -1) // Обработчик ошибок и инициализация сигнала SIGINT.
    {
        perror("sigaction");
        return 1;
    }
    if (sigaction(SIGTSTP, &sig_tstp, NULL) == -1) // Обработчик ошибок и инициализация сигнала SIGTSTP.
    {
        perror("sigaction");
        return 1;
    }

    show_grapgh();
    input_diap();

    system("clear");
    if (*mth == 0)
    {
        printf("\nFork\n");
        fork_method();
    }
    else if (*mth == 1)
    {
        printf("\nChord\n");
        chord_method();
    }
    else if (*mth == 2)
    {
        printf("\nNewton\n");
        newton_method();
    }
    else
    {
        printf("\nFork\n");
        fork_method();
    }

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}

// MEEEEEEEOW