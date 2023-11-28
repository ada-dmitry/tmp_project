#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;
char choice, *ch = &choice;

double f(double x);
// void find_zero();
void ctrlc_handler(int signum);
void choose_path();

double f(double x) // Функция обязана быть линейной
{
    return x + 4;
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
            // // find_zero();
            // printf("Ожидание сигнала Ctrl+C...\n");
            return;
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C');
    printf("Ошибка ввода, повторная попытка...");
    choose_path();
}

void ctrlc_handler(int signum)
{
    // printf("\nПолучен сигнал Ctrl+C\n");
    printf("\nТекущее приближение: %lf\n", *px);
    choose_path();
    // find_zero();
}

// void find_zero()
// {
//     //signal(SIGINT, ctrlc_handler);    
//     do
//     {
//         *px = (*pa + *pb) / 2; // метод деления отрезка пополам
//         printf("%lf %lf\n", *pa, *pb);
//         sleep(10);

//         if (f(*pa) * f(x) < 0)
//             *pb = x;
//         else if(f(*pb) * f(x) < 0)
//             *pa = x;

//         printf("Текущее приближение: %lf\n", *px);

//     } while(fabs(*pb - *pa) > eps && f(x) != 0);

//     printf("\nКорень уравнения: %lf\n", *px);
//     printf("Работа программы завершена.\n");
//     return;
// }

int main(){
    double tmp;

    struct sigaction sa;
    sa.sa_handler = ctrlc_handler;
    sa.sa_flags = SA_RESETHAND;
    sigemptyset(&sa.sa_mask);

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

    signal(SIGINT, ctrlc_handler);    
    do
    {
        *px = (*pa + *pb) / 2; // метод деления отрезка пополам
        printf("%lf %lf\n", *pa, *pb);
        sleep(1);

        if (f(*pa) * f(x) < 0)
            *pb = x;
        else if(f(*pb) * f(x) < 0)
            *pa = x;

        

    } while(fabs(*pb - *pa) > eps && f(x) != 0);

    printf("\nКорень уравнения: %lf\n", *px);
    printf("Работа программы завершена.\n");

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }   

    // printf("Ожидание сигнала Ctrl+C...\n");
    // signal(SIGINT, ctrlc_handler);
    // while(1) 
    //     {
    //         sleep(1);
    //         }
    return 0;
}