extern double a, b, eps, x, *pa, *pb, *px;
extern short method, *mth;
// Декларирование глобальных переменных,
// как внешних для последующего использования их во всей программе.
#define H 0.000001

double f(double x);
void ctrlc_handler(int signum);
void choose_path();
void fork_method();
void chord_method();
void newton_method();
double numerical_derivative(double x);