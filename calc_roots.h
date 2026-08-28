#pragma once

#include "base.h"

/*!
Считает корень для линейного уравнения
\param[in, out] quadr_eq структура квадратного уравнения, по которой ситается корень и записывается в структуру
*/
void calc_root_of_linear(struct quadratic_eq *quadr_eq);

/*!
Считает корень для квадратного уравнения
\param[in, out] quadr_eq структура квадратного уравнения, по которой ситаются корни и записываются в структуру
*/
void calc_roots_of_quadratic_eq(struct quadratic_eq *quadr_eq);

/*!
Считает значение квадратного уранения в точке x
\param quadr_eq структура квадратного уравнения, по которой ситается значение
\param x подстановочное значение
\return значение квадратного уранения в точке x
*/
double calc_value_of_quadr_eq(struct quadratic_eq *quadr_eq, double x);
