#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>

double a, b, eps, x, *pa = &a, *pb = &b, *px = &x;

double f(double x);
void ctrlc_handler(int signum);
void choose_path();
void fork_method();

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

int main(){

    double tmp;
    
    struct sigaction sa;
    sa.sa_handler = ctrlc_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);  

    printf("Введите интервал [a, b] и точность eps: ");
    scanf("%lf %lf %lf", pa, pb, &eps);

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }   
    
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