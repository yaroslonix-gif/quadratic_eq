#pragma once

#include "base.h"
#include <limits.h>

enum type {
    OPERATOR = 0,
    NUMBER = 1,
    VARIA = 2,
};

enum op {
    ADD = 0,
    SUB = 1,
    MULTIPLE = 2,
    DEV = 3,
    POWER = 4,
};

union value {
    enum op opera;
    char varia;
    int number;
};

struct node {
    enum type type_el;
    union value value_el;
    char name_el;
};

#define OPERATION(operator, A, B)  \
    switch (operator) \
    { \
    case ADD: \
        return A + B; \
        break; \
    \
    case SUB: \
        return A - B; \
        break; \
    \
    case MULTIPLE: \
        return A * B; \
        break; \
    \
    case DEV: \
        if (B == 0) return 0; \
        return A / B; \
        break; \
        \
    \
    case POWER: \
        return pow(A, B); \
        break; \
        \
    default: \
        break; \
    }

#define MIN(A, B) (A < B ? A : B)

#define MAX_LEN_EXPR 1024

struct conv_op {
    char opera_ch;
    enum op opera_op;
    int prioritet;
};

const struct conv_op conv_operators[] = {
    {'+', ADD, 0},
    {'-', SUB, 0},
    {'*', MULTIPLE, 1},
    {'/', DEV, 1},
    {'^', POWER, 2}
};

const size_t sz_conv_operators = sizeof(conv_operators) / sizeof(conv_operators[0]);

/*!
Преобразует символ математической операции в специальную константу
\param ch - символ математической операции
\return специальная константа типа enum op
*/
enum op convert_to_op(char ch);

/*!
Рассчитывает выражение из целых чисел и знаков
\param expr_n - выражение,
\param left - левая граница рассчета,
\param right - правая граница рассчета
\return значение выражения
*/
int calc(struct node expr_n[], int left, int right);

/*!
Находит индекс операции с наименьшим приоритетом
\param expr_n - выражение,
\param left - левая граница поиска,
\param right - правая граница поиска
\return индекс операции с наименьшим приоритетом или -1 (если не найден)
*/
int find_low_op(struct node expr_n[], int left, int right);

/*!
Преобразует char в int
\param ch - символ
\return int-овое значение
*/
int convert_to_digit(char ch);

/*!
Преобразует беззнаковую последовательность типа node в целое число
\param expr_n - последовательность,
\param left - левая граница преобразования,
\param right - правая граница преобразования
\return int-овое значение последовательность
*/
int convert_nodes_to_number(struct node expr_n[], int left, int right);

/*!
Преобразует строку в node-ы и считает значение
\param expr - входное выражение
\return значение выражения
*/
int solve_expr(char *expr);

/*!
Дописывает s2 к s1
\param s1, s2 - строки
*/
void add_str_s2_to_s1(char *s1, char *s2);

/*!
Очищает строковой буффер
\param buffer - буффер
*/
void free_buffer(char *buffer);

/*!
Убирает из выражения "*x", заменяя на "x"
\param expr - выражение
*/
void destroy_multiply_x(char *expr);

/*!
Обратное действие destroy_multiply_x
*/
void return_multiply_x(char *expr);

/*!
Извлекает из строки коэффициенты квадратного уравнения
\param eq - входное выражение;
\param a, b, c - коэффициенты квадратного уравнения
*/
void calc_coefs_from_expr(char eq[], int *a, int *b, int *c);

/*!
Извлекает из полного выражения коэффициенты квадратного уравнения в структуру
\param eq - входное выражение;
\param quadr_eq - структура квадратного ур-я
*/
void calc_coefs_of_quadratic_eq(char *eq, struct quadratic_eq *quadr_eq);

/*!
Разделяет полное выражение по знаку равно на две части(левую и правую)
\param eq - входное выражение;
\param left_eq, right_eq - левая и правая части
*/
void parsing_by_equal(char eq[], char left_eq[], char right_eq[]);

/*!
Рассчитывает приоритет математической операции
\param element - входная операция
\return приоритет математической операции
*/
int get_prioritet(struct node element);

/*!
Добавляет в буффер 1, если он пуст или содержит один знак(- или +)
\param cur_buf - буффер
*/
void push_1_after_add_or_sub_if_only_sign_in_buf(char *cur_buf);