#pragma once

//#define DEBUG_ON

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>


#define ZERO_BORDER 1e-6
#define DOUBLE_BORDER 1e9
#define LINE_LEN 100

#define RED         "\u001b[31m"
#define GREEN       "\u001b[32m"
#define YELLOW      "\u001b[33m"
#define RESET_COLOR "\u001b[0m"


#define LOGG(...) fprintf(LOG_F, __VA_ARGS__)

#define LOGG_TIME { time_t TIMER = time(NULL); \
    struct tm *G_TIMER = localtime(&TIMER); \
    LOGG("%d:%d:%d", G_TIMER->tm_hour, G_TIMER->tm_min, G_TIMER->tm_sec); }

#define ALOGG { LOGG_TIME \
    LOGG(":%s:%d:%s\n", __FILE__, __LINE__, __func__); } //autologg

#ifdef DEBUG_ON
    #define ASSERT(value, err) if (value == 0)\
        LOGG("%s%s:%d: Assertion error: %s%s\n", RED, __FILE__, __LINE__, prog_errors[err], RESET_COLOR);
#else
    #define ASSERT(value, err) ;
#endif

#define PRINT_COLOR(color, ...) {printf(color __VA_ARGS__); printf(RESET_COLOR);}

#define PRINT_ERR(...) PRINT_COLOR(RED, __VA_ARGS__)

#define SWAP(A, B, type) {type tmp = A; A = B; B = tmp;}


/*!
Файл для логгирования
*/
extern FILE *LOG_F;

enum cnt_of_roots {
    INF_ROOTS = -1,
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2
};

enum error {
    NULL_ERROR = 0,
    ERROR_OF_DIAPASON_OF_DOUBLE = 1,
};

enum input_output_mode {
    DEFAULT_INPUT_OUTPUT = -1,
    USER_INPUT_OUTPUT = 0,
    FILE_INPUT_OUTPUT = 1,
};

/*!
Тексты ошибок
*/
extern const char *prog_errors[];

struct quadratic_eq {
    double a;
    double b;
    double c;

    double discriminant;
    enum cnt_of_roots n_roots;
    double x1;
    double x2;
};

/*!
Приводит число к нулю, если оно отличается от 0 менее, чем на epsilon
\param num - входное число для приведения
\return выходное число (приведенное или нет)
*/
double flush_to_zero(double num);

/*!
Проверяет равенство чисел double с точностью до epsilon
\param a, b - входные числа для проверки равенства
\return 1/0 равны или нет
*/
int equal(double a, double b);

/*!
Проверяет входит ли число в максимальный диапазон
\param num - входное число для проверки вхождения
\return 1/0 входит или нет
*/
int check_diapason_error_for_double(double num);

/*!
Возводит a в степень pw
\param a, pw - входные числа: число и степень
\return число в степени
*/
double my_pow(double a, int pw);