#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>

#include "declare.h"



double f(double x) 
{
    return x - 16;
}

void fork_method()
{
    do
    {
        *px = (*pa + *pb) / 2; // метод деления отрезка пополам
        printf("%lf %lf\n", *pa, *pb);
        if (f(*pa) * f(x) <= 0)
            *pb = x;
        else if(f(*pb) * f(x) <= 0)
            *pa = x;
        sleep(1);
    } while(fabs(*pb - *pa) > eps && f(x) != 0);
    return;
}

void choose_path()
{
    char choice, *ch = &choice;

    struct termios term;
    
    // Получение текущих настроек терминала
    tcgetattr(0, &term);
    
    // Отключение символа ECHO
    term.c_lflag &= ~ECHO;
    
    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &term);

    printf("\nПродолжить поиск корня?\n");
    
    do
    {
        printf("\nC - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке: \n");
        *ch = getchar();
        *ch = getchar();
        switch (*ch)
        {
        case 'C':
        case 'c':
            return;
        case 'A':
        case 'a':
            printf("\nКорень уравнения: %lf\n", *px);
            printf("Работа программы завершена.\n");
            exit(0);
        case 'R':
        case 'r':
            // Включение символа ECHO
            term.c_lflag |= ECHO;
    
            // Применение новых настроек терминала
            tcsetattr(0, TCSANOW, &term);

            printf("Введите новые значения границ a и b: ");

            scanf("%lf%lf", pa, pb);

            if (f(*pa) * f(*pb) > 0)
            {
                printf("В указанных границах нуля функции не существует.\n");
                printf("Нажмите Ctrl+C и поменяйте границы\n");
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
            return;

        default:
            printf("Ошибка ввода, повторная попытка...");
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C' || *ch != 'a' || *ch != 'r' || *ch != 'c'); 

    // Включение символа ECHO
    term.c_lflag |= ECHO;
    
    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &term);
}

void ctrlc_handler(int signum)
{
    printf("\nТекущее приближение: %lf\n", *px);
    choose_path();
}