#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;

double f(double x);
void ctrlc_handler(int signum);
void choose_path();

double f(double x) 
{
    return exp(x)-1;
}

void choose_path()
{

    printf("\nПродолжить поиск корня?\
        (C - продолжить, A - закончить работу программы, R - начать поиск на другом отрезке): \n");
    do
    {
        *ch = getchar();
        switch (*ch)
        {
        case 'C':
            return;
        case 'A':
            printf("\nКорень уравнения: %lf\n", *px);
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
            return;
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C');
    printf("Ошибка ввода, повторная попытка...");
    choose_path();
}

void ctrlc_handler(int signum)
{
    printf("\nТекущее приближение: %lf\n", *px);
    choose_path();
}

int main(){
    double tmp;
    signal(SIGINT, ctrlc_handler);    

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

    do
    {
        *px = (*pa + *pb) / 2; // метод деления отрезка пополам
        printf("%lf %lf\n", *pa, *pb);
        sleep(1);

        if (f(*pa) * f(x) <= 0)
            *pb = x;
        else if(f(*pb) * f(x) <= 0)
            *pa = x;

        

    } while(fabs(*pb - *pa) > eps && f(x) != 0);

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    return 0;
}