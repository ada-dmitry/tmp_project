#include <stdbool.h>

extern double a, b, eps, x, *pa, *pb, *px;
extern short method, *mth, sig_flag;
extern struct termios default_term, modif_term;

// Декларирование глобальных переменных,
// как внешних для последующего использования их во всей программе.
#define H 0.000001
#define A -10000
#define B 10000
#define EPS 0.001

double f(double x);
void ctrlc_handler(int signum);
void ctrlz_handler(int signum);
void choose_path();
void fork_method();
void chord_method();
void newton_method();
double numerical_derivative(double x);
void __exit();
void show_grapgh();
void input_diap();
void change_diap();
// double input_number(char ch);
bool is_digit(char c);
bool is_sign(char c);
bool is_dot(char c);
double read_parse(char ch);
bool is_enter(char c);