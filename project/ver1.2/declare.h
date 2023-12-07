extern double a, b, eps, x, *pa, *pb, *px;
// Декларирование глобальных переменных,
// как внешних для последующего использования их во всей программе.

double f(double x);
void ctrlc_handler(int signum);
void choose_path();
void fork_method();
