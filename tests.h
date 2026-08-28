#pragma once

#include "base.h"
#include "input_output_mod.h"
#include "calc_roots.h"

/*!
Запускает один фиксированный тест
\param good_struct эталон ответа, n_test номер теста
\return 1/0 есть ли ошибка или нет
*/
int run_one_test_with_no_random_coef(const struct quadratic_eq *good_struct, size_t n_test);

/*!
Запускает один рандомный тест
\return 1/0 есть ли ошибка или нет
*/
int run_one_test_with_random_coef(void);

/*!
Запускает все тесты
*/
void test(void);
