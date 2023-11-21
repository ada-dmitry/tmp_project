#include <stdio.h>
#include <math.h>
#include <signal.h>

double f(double x)
{
    return x + x - 16; // пример уравнения, замените на свою функцию
}

void ctrlc_handler(int signum)
{
    printf("\nПолучен сигнал Ctrl+C\n");
    printf("Продолжить поиск корня? (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): ");
    
    
}

void vibor(char *ch)
{
    scanf(" %c", ch);
    if (*ch == 'A')
    {
        printf("Работа программы завершена.\n");
        exit(0);
    }
    else if (*ch == 'R')
    {
        break;
    }
}

int main()
{
    double a, b, x, eps;
    char choice;

    signal(SIGINT, ctrlc_handler);

    do
    {
        printf("Введите интервал [a, b] и точность eps: ");
        scanf("%lf %lf %lf", &a, &b, &eps);

        do
        {
            x = (a + b) / 2; // метод деления отрезка пополам
            if (f(a) * f(x) < 0)
            {
                b = x;
            }
            else
            {
                a = x;
            }
            printf("Текущее приближение: %lf\n", x);
            // printf("Продолжить вычисления? (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): ");

        } while (fabs(b - a) > eps);

        if (choice != 'R')
        {
            printf("Корень уравнения: %lf\n", x);
            printf("Работа программы завершена.\n");
            return 0;
        }
    } while (choice == 'R');

    return 0;
}
