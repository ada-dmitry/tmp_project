#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <termios.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <stdbool.h>

#include "declare.h"

double f(double x) // Функция, заданная пользователем, для которой происходит поиск корней.
{
    return x + 20;
}

void show_grapgh()
{
    char func[100] = "x+20";
    // Подключение GNU Plot
    FILE *gp = popen("gnuplot -persist", "w");

    if (gp == NULL)
    {
        printf("GNUerror\n");
        exit(3);
    }
    fprintf(gp, "set xrange [-5000:5000]\n");
    fprintf(gp, "set yrange [-5000:5000]\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot %s\n", func);
    fflush(gp); // очищение буфера вывода
    sig_flag = 1;
    printf("Вернитесь в терминал и нажмите Enter, чтобы продолжить...\n");
    getchar();
    sig_flag = 0;
    fprintf(gp, "exit\n");
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
    tcsetattr(0, TCSANOW, &modif_term);

    printf("\nПродолжить поиск корня?\n");
    printf("\nC - продолжить, A - закончить работу программы, \
R - начать поиск на другом отрезке: \n");

    do
    {
        // Очистка стандартного потока ввода от "лишних"
        __fpurge(stdin);
        choice = getchar();

        switch (choice)
        {
        case 'C': // Выбор вида "Продолжить вычисление".
        case 'c':
            sig_flag = 0;
            system("clear");
            return; // Выход из функции без каких-либо изменений.

        case 'A': // Выбор вида "Завершить вычисление".
        case 'a':
            printf("\nКорень уравнения: %lf\n", *px);
            printf("Работа программы завершена.\n");
            __exit(); // Принудительное завершение программы с выводом текущего приближения корня.

        case 'R': // Выбор вида "Продолжить вычисления с новым отрезком".
        case 'r':
            change_diap();
            return;

        default:
            system("clear");
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
        system("clear");
        printf("\nТекущее приближение: %lf\n", *px);
        choose_path();
    }
    return;
}

void ctrlz_handler(int signum)
{
    printf("Аварийное завершение программы, возврат к настройкам по умолчанию...");
    __exit();
}

void __exit()
{
    tcsetattr(0, TCSANOW, &default_term); // Применение новых настроек терминала
    exit(1);
}

void input_diap()
{
    double tmp;
    sig_flag = 1;

    system("clear");
    printf("\n\nТекущие значения: [%g] : %g | eps = %g\n\n\
Введите левую границу отрезка и нажмите Enter (по умолчанию: %g): ",
           *pa, *pb, eps, *pa);

    *pa = read_parse('A');

    system("clear");
    printf("\nТекущие значения: %g : [%g] | eps = %g\n\n\
Введите правую границу отрезка и нажмите Enter (по умолчанию: %g): ",
           *pa, *pb, eps, *pb);

    *pb = read_parse('B');

    system("clear");
    printf("\nТекущие значения: %g : %g | eps = [%g]\n\n\
Введите точность вычисления и нажмите Enter (по умолчанию: %g): ",
           *pa, *pb, eps, eps);

    eps = read_parse('E');

    system("clear");
    __fpurge(stdin);
    printf("\nВыберите метод вычисления корня и нажмите Enter:\
 0 - метод вилки, 1 - метод хорд, 2 - метод касательных\n(По умолчанию: 0): ");
    *mth = getchar();

    if (*pa > *pb) // Если пользователь перепутал границы
    {
        printf("Границы указаны неверно, запуск задачи с исправленным условием\n");
        tmp = *pa;
        *pa = *pb;
        *pb = tmp;
    }

    if (f(*pa) * f(*pb) > 0)
    {
        sig_flag = 0;
        printf("В указанных границах нуля функции не существует.\n");
        printf("Поменяйте границы\n");
        change_diap();
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
            __exit();
        }
        else
        {
            printf("Нажмите Ctrl+C и поменяйте границы\n");
        }
    }
    sig_flag = 0;
    return;
}

void change_diap()
{
    // Возврат к обычному выводу
    tcsetattr(0, TCSANOW, &default_term);

    system("clear");

    printf("Введите новое значение левой границы: ");
    *pa = read_parse('A');
    printf("Введите новое значение правой границы: ");
    *pb = read_parse('B');

    if (f(*pa) * f(*pb) > 0)
    {
        printf("\nВ указанных границах нуля функции не существует.\n");
        printf("Поменяйте границы\n");
        choose_path();
    }
    if (*pa == *pb)
    {
        if (f(*pa) == 0)
        {
            printf("\nКорень уравнения: %lf\n", *pa);
            printf("Работа программы завершена.\n");
            __exit();
        }
        else
        {
            printf("Нажмите Ctrl+C и поменяйте границы\n");
        }
    }

    sig_flag = 0;
    return;
}

bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

bool is_sign(char c)
{
    return (c == '+' || c == '-');
}

bool is_dot(char c)
{
    return (c == '.');
}

bool is_enter(char c)
{
    return (c == '\n');
}

double read_parse(char ch)
{
    char input[50];
    double number;
    fgets(input, sizeof(input), stdin);
    __fpurge(stdin);

    int i = 0;
    bool is_valid = true;
    bool has_dot = false;
    // Проверка первого символа, который может быть знаком или переносом строки
    if (is_sign(input[0]))
    {
        i++;
    }
    else if (is_enter(input[0]))
    {
        switch (ch)
        {
        case 'A':
            return A;
        case 'B':
            return B;
        case 'E':
            return EPS;
        }
    }
    // Проверка символов, формирующих цифры числа
    while (input[i] != '\n')
    {
        if (is_digit(input[i]))
        {
            i++;
            continue;
        }
        else if (is_dot(input[i]) && !has_dot)
        {
            has_dot = true;
            i++;
            continue;
        }
        else
        {
            is_valid = false;
            break;
        }
    }

    if (is_valid)
    {
        number = atof(input);
        return number;
    }
    else
    {
        printf("Ошибка: некорректный ввод\nЗавершение выполнения программы...\n");
        __exit();
    }
    return 1;
}
