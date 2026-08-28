#pragma once

#include "base.h"

/*!
Печатает решение квадратного уранения в поток
\param quadr_eq - квадратное уравнение
\param fpout - поток для записи
*/
void print_roots_of_quadratic_eq(const struct quadratic_eq *quadr_eq, FILE *fpout);

/*!
Отбрасывает конец строки потока
\param fpin - поток
\param push_n - параметр для возврата символа конца строки в поток (1/0)
*/
void skip_entered_data(FILE *fpin, int push_n);

/*!
Печатает в терминал линию из равно
*/
void print_line(void);

/*!
Устанавливает потоки ввода и вывода
\param mode - тип потоков
\param fp_quadr_eq_in - поток ввода
\param fp_quadr_eq_out - поток вывода
\param argc, argv - аргументы main
*/
void set_input_and_output(enum input_output_mode *mode, FILE **fp_quadr_eq_in, FILE **fp_quadr_eq_out, int argc, char *argv[]);

/*!
Pfrhsdftn потоки ввода и вывода
\param fp_quadr_eq_in - поток ввода
\param fp_quadr_eq_out - поток вывода
*/
void close_input_and_output(FILE *fp_quadr_eq_in, FILE *fp_quadr_eq_out);

/*!
Проверяет буффер на допустимые символы
\param buf - буффер
\return 1/0 (соответсвует или нет)
*/
int check_buf_for_valid(char *buf);

/*!
"Красивая" печать квадратного уравнения в терминал
\param quadr_eq - квадратное уравнение
*/
void print_quadratic_eq(struct quadratic_eq quadr_eq);

/*!
Читает коэффициенты квадратного уравнения из строки формата (1x^2-1.2x+x...)
\param buf - буффер
\param quadr_eq - квадратное уравнение
\return количество успешных считываний
*/
int scan_sq_eq(char *buf, struct quadratic_eq *quadr_eq);

/*!
Печатает решение квадратного уранения в строку
\param quadr_eq - квадратное уравнение
\param buf_out - строка для записи
*/
void print_roots_of_quadratic_eq_to_buf(const struct quadratic_eq *quadr_eq, char *buf_out);