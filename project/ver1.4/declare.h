extern double a, b, eps, x, *pa, *pb, *px;
extern short method, *mth, sig_flag;

// Декларирование глобальных переменных,
// как внешних для последующего использования их во всей программе.
#define H 0.000001
#define A 0
#define B 100000000
#define EPS 0.001

double f(double x);
void ctrlc_handler(int signum);
void choose_path();
void fork_method();
void chord_method();
void newton_method();
double numerical_derivative(double x);
void exit_777();
void show_grapgh();
void input_diap();
