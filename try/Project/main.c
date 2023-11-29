#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double f(double x);
void find_zero();
void ctrlc_handler(int signum);
void choose_path();
// int cnt_root();

double a, b, eps, x, *pa = &a, *pb = &b;
// char choice, *ch = &choice;

int main()
{
    double tmp;

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

    find_zero();
    printf("Ожидание сигнала Ctrl+C...\n");
    signal(SIGINT, ctrlc_handler);
    while (1)
    {
        sleep(1);
    }

    return 0;
}

double f(double x) // Функция обязана быть линейной
{
    return x + x - 16;
}

void find_zero()
{    
    double x; 
    x = (*pa + *pb) / 2; // метод деления отрезка пополам
    if (f(*pa) * f(x) < 0)
        *pb = x;
    else
        *pa = x;
        printf("Текущее приближение: %lf\n", x);
    if (fabs(*pb - *pa) < eps)
    {
        printf("\nКорень уравнения: %lf\n", x);
        printf("Работа программы завершена.\n");
        exit(0);
    }
    else
    {
        return;
    }
}

void ctrlc_handler(int signum)
{
    printf("\nПолучен сигнал Ctrl+C\n");
    find_zero();
    choose_path();
}

void choose_path()
{
    char choice, *ch = &choice;
    printf("\nПродолжить поиск корня?\
        (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): \n");
    do
    {
        *ch = getchar();
        switch (*ch)
        {
        case 'C':
            // find_zero();
            printf("Ожидание сигнала Ctrl+C...\n");
            return;
        case 'A':
            printf("Работа программы завершена.\n");
            exit(0);
        case 'R':
            printf("Введите новые значения границ a и b: ");
            scanf("%lf %lf", pa, pb);
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
            find_zero();
            printf("Ожидание сигнала Ctrl+C...\n");
            return;
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C');
    printf("Ошибка ввода, повторная попытка...");
    choose_path();
}

