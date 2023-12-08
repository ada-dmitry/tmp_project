#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
// #include <signal.h>
#include <termios.h>
#include <stdio_ext.h>

#include "declare.h"

double f(double x) // Функция, заданная пользователем, для которой происходит поиск корней.
{
    return x - x * x + 1349;
}

void show_grapgh()
{
    struct termios old_term, new_term;

    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    char func[100] = "x - x * x + 1349";

    // Подключение GNU Plot
    FILE *gp = popen("gnuplot -persist", "w");

    if (gp == NULL)
    {
        printf("GNUerror\n");
        exit(3);
    }
    fprintf(gp, "set xrange [-500:500]\n");
    fprintf(gp, "set yrange [0:500]\n");
    fprintf(gp, "plot %s, 0\n", func);
    fflush(gp); // очищение буфера вывода
    sig_flag = 1;
    printf("Нажмите любую клавишу, чтобы продолжить...\n");
    getchar();
    sig_flag = 0;
    fprintf(gp, "exit\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
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
    modif_term.c_lflag &= ~(ECHO | ICANON);
    // Применение новых настроек терминала
    tcsetattr(0, TCSANOW, &modif_term);

    printf("\nПродолжить поиск корня?\n");
    printf("\nC - продолжить, A - закончить работу программы, \
R - начать поиск на другом отрезке: \n");

    do
    {
        // Очистка стандартного потока ввода от "лишних"
        __fpurge(stdin);
        choice = getchar();
        // scanf("%c", &choice); // Запись буквы, введенной пользователем в переменную choice.
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
                    exit_777();
                }
                else
                {
                    printf("Нажмите Ctrl+C и поменяйте границы\n");
                }
            }

            sig_flag = 0;
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

void ctrlz_handler(int signum)
{
    printf("Аварийное завершение программы, возврат к настройкам по умолчанию...");
    exit_777();
}

void exit_777()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
    exit(1);
}

void input_diap()
{
    double tmp;
    char input[50];
    sig_flag = 1;
    printf("\nПоочередно вводите через Enter: левую границу, правую и точность\n\
(или нажимайте Enter для значений по умолчанию: -10000/10000/0.0001): ");

    // printf("Введите левую границу отрезка (или нажмите Enter для значения по умолчанию): ");
    fgets(input, sizeof(input), stdin);

    if (input[0] == '\n')
    {
        *pa = A; // значение по умолчанию
    }
    else
    {
        sscanf(input, "%lf", pa);
    }

    // printf("Введите правую границу отрезка (или нажмите Enter для значения по умолчанию): ");
    fgets(input, sizeof(input), stdin);

    if (input[0] == '\n')
    {
        *pb = B; // значение по умолчанию
    }
    else
    {
        sscanf(input, "%lf", pb);
    }

    // printf("Введите точность вычисления (или нажмите Enter для значения по умолчанию): ");
    fgets(input, sizeof(input), stdin);

    if (input[0] == '\n')
    {
        eps = EPS; // значение по умолчанию
    }
    else
    {
        sscanf(input, "%lf", &eps);
    }

    __fpurge(stdin);
    printf("Выберите метод вычисления корня и нажмите Enter:\
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
        choose_path();
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
            exit_777();
        }
        else
        {
            printf("Нажмите Ctrl+C и поменяйте границы\n");
        }
    }
    sig_flag = 0;
    return;
}