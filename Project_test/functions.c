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
    sig_flag = 0;
    double left = *pa, right = *pb;
    right = left;
    do
    {
        printf("%lf %lf\n", left, right);
        left = right;
        right = left - f(left) / numerical_derivative(left);
        // Используем численно вычисленную производную
        *px = right;
        sleep(1);
    } while (fabs(right - left) > eps && f(x) != 0);
    return;
}

void chord_method()
{
    sig_flag = 0;
    double left = *pa, right = *pb;
    *px = left;
    while ((right - left) > eps && f(x) != 0)
    {
        // Находим точку пересечения хорды с осью x
        *px = (left * f(right) - right * f(left)) / (f(right) - f(left));
        printf("%lf %lf\n", left, right);
        // Проверяем знаки функции в точках a и x
        if (f(*px) == 0)
            break;
        else if (f(*px) * f(left) < 0)
            right = *px;
        else
            left = *px;
        sleep(1);
    }
    return;
}

void fork_method() // Функция поиска корней, использующая метод вилки.
{
    sig_flag = 0;
    double left = *pa, right = *pb;
    do
    {
        *px = (left + right) / 2; // метод деления отрезка пополам

        // Печать границ для отладки и для отслеживания хода вычислений
        printf("%lf %lf\n", left, right);

        // Проверка, какой из отрезков [a, x] или [x, b] подходит под условие существования корня,
        // т.е. различие знаков на границе.
        if (f(left) * f(x) <= 0)
            right = x;
        else if (f(right) * f(x) <= 0)
            left = x;

        sleep(1); // Принудительное ожидание программой, для возможности прерывания пользователем.

    } while (fabs(right - left) > eps && f(x) != 0);
    return;
}

void choose_path() // Функция выбора действия после вызова сигнала.
{
    sig_flag = 1;

    char choice, *ch = &choice;

    struct termios original_term, modified_term; // Задание структуры, определяющей терминал Linux.

    tcgetattr(STDIN_FILENO, &original_term);
    modified_term = original_term;
    modified_term.c_lflag &= ~(ICANON | ECHO); // отключить канонического режим и отображение ввода
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_term);

    printf("\nПродолжить поиск корня?\n");
    printf("\nC - продолжить, A - закончить работу программы, \
R - начать поиск на другом отрезке: \n");

    do
    {
        // Очистка стандартного потока ввода от "лишних"
        __fpurge(stdin);
        *ch = getchar(); // Запись буквы, введенной пользователем в переменную choice.

        switch (*ch)
        {
        case 'C': // Выбор вида "Продолжить вычисление".
        case 'c':
            tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
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
            return; // Выход из функции без каких-либо изменений.

        case 'A': // Выбор вида "Завершить вычисление".
        case 'a':
            printf("\nКорень уравнения: %lf\n", *px);
            printf("Работа программы завершена.\n");
            exit(0); // Принудительное завершение программы с выводом текущего приближения корня.

        case 'R': // Выбор вида "Продолжить вычисления с новым отрезком".
        case 'r':

            tcsetattr(STDIN_FILENO, TCSANOW, &original_term);

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
            printf("Ошибка ввода, повторная попытка...\n");
            __fpurge(stdin);
        }
    } while (*ch != 'A' || *ch != 'R' || *ch != 'C' || *ch != 'a' || *ch != 'r' || *ch != 'c');

    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);

    sig_flag = 0;
}

void ctrlc_handler(int signum) // Функция, заменяющая стандартный алгоритм для сигнала SIGINT.
{
    if (sig_flag == 0)
    {
        printf("\nТекущее приближение: %lf\n", *px);
        choose_path();
    }
}