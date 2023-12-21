/*
 * $gcc -Wall main.c -o main -ldl
 * $./main sin\(x\) 1.62
 *  0.998790
 * $./main sin\(x\)*exp\(x\) 1.62
 *  5.046975
 */

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define BEGIN "#include <math.h>\ndouble func(double x){\nreturn "
#define END ";\n}\n"
#define MAKELIB "gcc -c -Wall func.c -o func.o && gcc -L. -shared func.o -o libfunc.so -lm"

int main(int argc, char *argv[])
{
    void *handle;
    double (*fun)(double);
    char *error;
    FILE *f;

    f = fopen("func.c", "w");
    fprintf(f, "%s%s%s", BEGIN, argv[1], END);
    fclose(f);
    system(MAKELIB);

    handle = dlopen("./libfunc.so", RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    dlerror();

    fun = (double (*)(double))dlsym(handle, "func");

    error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }
    printf("%f\n", (*fun)(atof(argv[2])));
    dlclose(handle);
    exit(EXIT_SUCCESS);
}