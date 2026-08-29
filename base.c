#include "base.h"

FILE *LOG_F = stdin;

const char *prog_errors[] = {
    "NULL_error",
    "error_of_diapason_double"
};

double flush_to_zero(double num) {
    if (fabs(num) < ZERO_BORDER)
        return 0;

    return num;
}

int check_diapason_error_for_double(double num) {
    if (-DOUBLE_BORDER > num || num > DOUBLE_BORDER) {
        fprintf(stderr, "%smin = %.4lf, max = %.4lf%s\n", RED, -DOUBLE_BORDER, DOUBLE_BORDER, RESET_COLOR);

        return 0;
    }

    return 1;
}

double my_pow(double a, int pw) {
    double res = 1;

    for (int i = 0; i < pw; i++)
        res *= a;

    return res;
}

int equal(double a, double b) {
    return fabs(a - b) <= ZERO_BORDER;
}