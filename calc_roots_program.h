#pragma once

#define TX_CONSOLE_MODE SW_HIDE

#include <TXlib.h>
#include "base.h"
#include "input_output_mod.h"
#include "calc_roots.h"
#include "tests.h"
#include "expr_solver.h"


struct button {
    double x1, y1, x2, y2;
    char *but_text;
};

enum mouse_clicks {
    LEFT_CLICK = 1,
    RIGHT_CLICK = 2,
    LEFT_AND_RIGHT_CLICK = 3
};

extern const struct button buttons[];

extern size_t size_of_buttons;

/*!
Отрисовывает элементы управления(кнопки)
*/
void draw_buttons();

/*!
Обрабатывает нажатие кнопки
\return текст кнопки или NULL, если никакая кнопка не была нажата
*/
char *check_buttons();

/*!
Печатает на окне теущий буффер выражения, упрощенное выражение, решение уравнения
\param buffer_eq - теущий буффер выражения,
\param simple_expr - упрощенное выражение,
\param last_solution - решение уравнения
*/
void update_window(char *buffer_eq, char *simple_expr, char *last_solution);

/*!
Выполняет backspace
\param buffer_eq - теущий буффер выражения, 
\param now_buf_ind - текущая заполненность буффера,
\param last_sz_el - длина стирания
*/
void do_backspace(char *buffer_eq, int *now_buf_ind, int last_sz_el);

/*!
Очищает буффер и сбрасывает заполненность
\param buffer_eq - теущий буффер выражения
\param now_buf_ind - текущая заполненность буффера
*/
void reset_buffer(char *buffer_eq, int *now_buf_ind);