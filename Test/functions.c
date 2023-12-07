#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>
#include <stdio_ext.h>

#include "declare.h"

double f(double x) // Функция, заданная пользователем, для которой происходит поиск корней.
{
    return x - 125;
}

void show_grapgh()
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    char func[100] = "x - 125";
    // Подключение GNU Plot
    FILE *gp = popen("gnuplot -persist", "w");

    if (gp == NULL)
    {
        printf("GNUerror\n");
        exit(3);
    }
    fprintf(gp, "set xrange [-500:500]\n");
    fprintf(gp, "set yrange [0:500]\n");
    fprintf(gp, "plot %s\n", func);
    fflush(gp); // очищение буфера вывода
    printf("Введите любую букву, чтобы продолжить...\n");
    getchar();
    fprintf(gp, "exit\n");
    term.c_lflag |= (ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    return;
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
    sig_flag = 1;

    char choice;

    struct termios default_term, modif_term; // Задание структуры, определяющей терминал Linux.

    // Получение текущих настроек терминала
    tcgetattr(0, &default_term);
    modif_term = default_term;
    // Отключение символа ECHO
    modif_term.c_lflag &= ~ECHO;
    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &modif_term);

    printf("\nПродолжить поиск корня? Напечатайте букву и нажмите Enter\n");
    printf("\nC - продолжить, A - закончить работу программы, \
R - начать поиск на другом отрезке: \n");

    do
    {
        // Очистка стандартного потока ввода от "лишних"
        __fpurge(stdin);
        scanf("%c", &choice); // Запись буквы, введенной пользователем в переменную choice.

        switch (choice)
        {
        case 'C': // Выбор вида "Продолжить вычисление".
        case 'c':
            sig_flag = 0;
            return; // Выход из функции без каких-либо изменений.

        case 'A': // Выбор вида "Завершить вычисление".
        case 'a':
            printf("\nКорень уравнения: %lf\n", *px);
            printf("Работа программы завершена.\n");
            exit_777(); // Принудительное завершение программы с выводом текущего приближения корня.

        case 'R': // Выбор вида "Продолжить вычисления с новым отрезком".
        case 'r':

            // Возврат к обычному выводу
            tcsetattr(0, TCSANOW, &default_term);

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
                    exit_777();
                }
                else
                {
                    printf("Нажмите Ctrl+C и поменяйте границы\n");
                }
            }

            sig_flag = 0;
            __fpurge(stdin);
            return;

        default:
            printf("Ошибка ввода, повторная попытка...\n");
            __fpurge(stdin);
        }
    } while (choice != 'A' || choice != 'R' || choice != 'C' || choice != 'a' || choice != 'r' || choice != 'c' || choice != 'M' || choice != 'm');

    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &default_term);

    sig_flag = 0;
}

void ctrlc_handler(int signum) // Функция, заменяющая стандартный алгоритм для сигнала SIGINT.
{
    if (sig_flag == 0)
    {
        printf("\nТекущее приближение: %lf\n", *px);
        choose_path();
    }
    return;
}

void exit_777()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
    exit(1);
}
