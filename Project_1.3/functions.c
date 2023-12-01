#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>
#include <stdio_ext.h>

#include "declare.h"

double f(double x) // Функция, заданная пользователем, для которой происходит поиск корней.
{
    return x * x * x - 125;
}

// Приближенное вычисление производной по методу конечных разностей
double numerical_derivative(double x)
{
    return (f(x + H) - f(x)) / H;
}

void newton_method()
{
    *pb = *pa;
    do
    {
        printf("%lf %lf\n", *pa, *pb);
        *pa = *pb;
        *pb = *pa - f(*pa) / numerical_derivative(*pa);
        // Используем численно вычисленную производную
        *px = *pb;
        sleep(1);
    } while (fabs(*pb - *pa) > eps && f(x) != 0);
    return;
}

void chord_method()
{
    *px = *pa;
    while ((*pb - *pa) > eps && f(x) != 0)
    {
        // Находим точку пересечения хорды с осью x
        *px = (*pa * f(*pb) - *pb * f(*pa)) / (f(*pb) - f(*pa));
        printf("%lf %lf\n", *pa, *pb);
        // Проверяем знаки функции в точках a и x
        if (f(*px) == 0)
            break;
        else if (f(*px) * f(*pa) < 0)
            *pb = *px;
        else
            *pa = *px;
        sleep(1);
    }
    return;
}

void fork_method() // Функция поиска корней, использующая метод вилки.
{
    do
    {
        *px = (*pa + *pb) / 2; // метод деления отрезка пополам

        // Печать границ для отладки и для отслеживания хода вычислений
        printf("%lf %lf\n", *pa, *pb);

        // Проверка, какой из отрезков [a, x] или [x, b] подходит под условие существования корня,
        // т.е. различие знаков на границе.
        if (f(*pa) * f(x) <= 0)
            *pb = x;
        else if (f(*pb) * f(x) <= 0)
            *pa = x;

        sleep(1); // Принудительное ожидание программой, для возможности прерывания пользователем.

    } while (fabs(*pb - *pa) > eps && f(x) != 0);
    return;
}

void choose_path() // Функция выбора действия после вызова сигнала.
{
    char choice, *ch = &choice;

    struct termios term; // Задание структуры, определяющей терминал Linux.

    // Получение текущих настроек терминала
    tcgetattr(0, &term);

    // Отключение символа ECHO
    term.c_lflag &= ~ECHO;

    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &term);

    printf("\nПродолжить поиск корня?\n");
    printf("\nC - продолжить, A - закончить работу программы, \
R - начать поиск на другом отрезке, M - выбрать другой способ решения: \n");

    // Очистка стандартного потока ввода от "лишних"
    __fpurge(stdin);

    do
    {
        *ch = getchar(); // Запись буквы, введенной пользователем в переменную choice.

        switch (*ch)
        {
        case 'C': // Выбор вида "Продолжить вычисление".
        case 'c':
            return; // Выход из функции без каких-либо изменений.

        case 'A': // Выбор вида "Завершить вычисление".
        case 'a':
            printf("\nКорень уравнения: %lf\n", *px);
            printf("Работа программы завершена.\n");
            exit(0); // Принудительное завершение программы с выводом текущего приближения корня.

        case 'R': // Выбор вида "Продолжить вычисления с новым отрезком".
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
        case 'M':
        case 'm':
            // Включение символа ECHO
            term.c_lflag |= ECHO;

            // Применение новых настроек терминала
            tcsetattr(0, TCSANOW, &term);
            printf("Выберите метод вычисления корня:\
 0 - метод вилки, 1 - метод хорд, 2 - метод касательных\n(По умолчанию: 0)\n");
            scanf("%hd", mth);

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
            return;

        default:
            printf("Ошибка ввода, повторная попытка...\n");
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C' || *ch != 'a' || *ch != 'r' || *ch != 'c' || *ch != 'M' || *ch != 'm');

    // Включение символа ECHO
    term.c_lflag |= ECHO;

    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &term);
}

void ctrlc_handler(int signum) // Функция, заменяющая стандартный алгоритм для сигнала SIGINT.
{
    printf("\nТекущее приближение: %lf\n", *px);
    choose_path();
}