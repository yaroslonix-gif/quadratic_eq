#pragma once

#define TX_CONSOLE_MODE SW_HIDE

#include <TXlib.h>
#include "base.h"
#include "input_output_mod.h"
#include "calc_roots.h"
#include "tests.h"
#include "expr_solver.h"


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGTH 600


struct button {
    double x1, y1, x2, y2;
    char *but_text;
};

enum mouse_clicks {
    LEFT_CLICK = 1,
    RIGHT_CLICK = 2,
    LEFT_AND_RIGHT_CLICK = 3
};

enum prog_mode {
    DEFAULT_MODE = 0,
    DYNAMIC_MODE = 1
};

struct name_of_prog_mode {
    enum prog_mode mode_mode;
    char *mode_name;
};

struct slider {
    double x1, y1, x2, y2;
    char *slider_text;
    double min_value, max_value;
};

extern const struct button buttons[];

extern const size_t size_of_buttons;

extern const struct name_of_prog_mode prog_modes[];

extern const size_t size_of_prog_modes;

extern const struct button mode_button;

extern const struct slider sliders[];

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
Печатает на окне теущий буффер выражения, упрощенное выражение, решение уравнения при режиме DEFAULT
\param buffer_eq - теущий буффер выражения
\param simple_expr - упрощенное выражение
\param last_solution - решение уравнения
\param quadr_eq - квадратное уравнение
\param cur_mode - текущий режим программы
*/
void update_window_default_mode(char *buffer_eq, char *simple_expr, char *last_solution, struct quadratic_eq *quadr_eq, enum prog_mode cur_mode);

/*!
Выполняет backspace
\param buffer_eq - теущий буффер выражения
\param now_buf_ind - текущая заполненность буффера
\param last_sz_el - длина стирания
*/
void do_backspace(char *buffer_eq, int *now_buf_ind, int last_sz_el);

/*!
Очищает буффер и сбрасывает заполненность
\param buffer_eq - теущий буффер выражения
\param now_buf_ind - текущая заполненность буффера
*/
void reset_buffer(char *buffer_eq, int *now_buf_ind);

/*!
Отрисовывает график квадратного трехчлена
\param quadr_eq - квадратное уравнение
*/
void draw_grafic(struct quadratic_eq *quadr_eq);

/*!
Отрисовывает кнопку смены режима и текущий режим
\param cur_mode - текущий режим программы
*/
void draw_mode_button(const enum prog_mode cur_mode);

/*!
Возвращает индекс текущего режима программы по списку режимов
\param cur_mode - текущий режим программы
\return индекс текущего режима
*/
int index_of_cur_mode(const enum prog_mode cur_mode);

/*!
Проверяет нажатие кнопки смены режима программы
\param cur_mode - текущий режим программы
*/
void check_button_mode(enum prog_mode *cur_mode);

/*!
Режим DEFAULT окно с кнопками для ввода квадратного ур-я и отображения решений в текстовом и графическом выводе
\param buffer_eq - теущий буффер выражения
\param simple_expr - упрощенное выражение
\param last_solution - решение уравнения
\param quadr_eq - квадратное уравнение
\param pt_now_buf_ind - указатель на текущую заполненность программы
\param cur_mode - текущий режим программы
*/
void run_default_mode(char *buffer_eq, char *simple_expr, char *last_solution, struct quadratic_eq *quadr_eq, int *pt_now_buf_ind, enum prog_mode cur_mode);

/*!
Отрисовка окна программы в режиме DYNAMIC
\param sliders_value - значения слайдеров
\param simple_expr - упрощенное выражение
\param last_solution - решение уравнения
\param pt_quadr_eq - указатель на квадратное уравнение
\param cur_mode - текущий режим программы
*/
void update_window_dynamic_mode(double sliders_value[], char *simple_expr, char *last_solution, struct quadratic_eq *pt_quadr_eq, enum prog_mode cur_mode);

/*!
Режим DYNAMIC окно с слайдерами для установки коэффициентов квадратного уравнения, с текстовым и графическим выводом
\param sliders_value - значения слайдеров
\param simple_expr - упрощенное выражение
\param last_solution - решение уравнения
\param pt_quadr_eq - указатель на квадратное уравнение
\param cur_mode - текущий режим программы
*/
void run_dynamic_mode(double sliders_value[], char *simple_expr, char *last_solution, struct quadratic_eq *pt_quadr_eq, enum prog_mode cur_mode);

/*!
Отрисовывает элемент управления(кнопку)
\param but - кнопка
*/
void draw_button(const struct button but);

/*!
Отрисовывает элемент управления(слайдер)
\param cur_slider - слайдер
\param value - значение слайдера
*/
void draw_slider(const struct slider cur_slider, double value);

/*!
Обрабатывает изменение значений слайдеров
\param sliders_value - значения слайдеров
*/
void check_sliders(double sliders_value[]);

/*!
Отрисовывает элементы управления(слайдеры)
\param sliders_value - значения слайдеров
*/
void draw_sliders(double sliders_value[]);

/*!
Передает значения слайдеров в квадратное уравнение
\param sliders_value - значения слайдеров
\param pt_quadr_eq - указатель на квадратное уравнение
*/
void set_coefs_for_eq_from_sliders(double sliders_value[], struct quadratic_eq *pt_quadr_eq);