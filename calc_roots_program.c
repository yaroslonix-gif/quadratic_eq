#include "calc_roots_program.h"

//=========================================================================================================================
const struct button buttons[] = {
    {  0, 300, 100, 400, "1"    },
    {  0, 400, 100, 500, "2"    },
    {  0, 500, 100, 600, "3"    },
    {100, 300, 200, 400, "4"    },
    {100, 400, 200, 500, "5"    },
    {100, 500, 200, 600, "6"    },
    {200, 300, 300, 400, "7"    },
    {200, 400, 300, 500, "8"    },
    {200, 500, 300, 600, "9"    },
    {300, 300, 400, 400, "+"    },
    {300, 400, 400, 500, "-"    },
    {300, 500, 400, 600, "0"    },
    {400, 300, 500, 400, "*"    },
    {400, 400, 500, 500, "/"    },
    {400, 500, 500, 600, "^"    },
    {500, 300, 600, 400, "<-"   },
    {500, 400, 600, 500, "x"    },
    {500, 500, 600, 600, "x^2"  },
    {600, 300, 700, 400, "solve"},
    {600, 400, 700, 500, "AC"   },
    {600, 500, 700, 600, "="    }
};

const size_t size_of_buttons = sizeof(buttons) / sizeof(buttons[0]);

//=========================================================================================================================
const struct name_of_prog_mode prog_modes[] = {
    {.mode_mode = DEFAULT_MODE, .mode_name = "DEFAULT_MODE"},
    {.mode_mode = DYNAMIC_MODE, .mode_name = "DYNAMIC_MODE"},
    {.mode_mode = RANDOM_MODE, .mode_name = "RANDOM_MODE"}
};

const size_t size_of_prog_modes = sizeof(prog_modes) / sizeof(prog_modes[0]);

const struct button mode_button = {0, 0, 125, 40, "CHANGE_MODE"};

//=========================================================================================================================
const struct slider sliders[] = {
    {100, 300, 500, 325, "a",   -2,   2},
    {100, 400, 500, 425, "b", -100, 100},
    {100, 500, 500, 525, "c", -100, 100}
};

const size_t size_of_sliders = sizeof(sliders) / sizeof(sliders[0]);

//=========================================================================================================================
const struct button rand_buttons[] = {
    { 100, 500,  400, 540, "STOP COEFS"},
    { 500, 500,  700, 540, "STOP X"    },
    {1100, 560, 1200, 600, "RESTART"   }
};

const size_t size_of_rand_buttons = sizeof(rand_buttons) / sizeof(rand_buttons[0]);

const struct name_of_rand_status rand_buttons_names[] = {
    {  .rand_st = RAND_COEFS, .rand_st_name = "STOP COEFS"},
    {      .rand_st = RAND_X, .rand_st_name = "STOP X"    },
    {.rand_st = RAND_RESTART, .rand_st_name = "RESTART"   }
};

//=========================================================================================================================
int main() {
    srand(time(0));
    enum prog_mode cur_mode = DEFAULT_MODE;
    LOG_F = fopen("log.txt", "w");
    ASSERT(LOG_F != NULL, NULL_ERROR);
    ALOGG;
    
    test();

    struct quadratic_eq quadr_eq = {0};

    txCreateWindow(WINDOW_WIDTH, 600);
    txTextCursor(0);
    txSetFillColor(TX_BLACK);

    char buffer_eq[MAX_LEN_EXPR] = {0};
    char simple_expr[MAX_LEN_EXPR] = {0};
    char last_solution[MAX_LEN_EXPR] = {0};
    int now_buf_ind = 0;

    double sliders_value[size_of_sliders] = {0};

    update_window_default_mode(buffer_eq, simple_expr, last_solution, &quadr_eq, cur_mode);

    while (1) {

        while (txMouseButtons() != LEFT_CLICK)
            txSleep(10);
        
        check_button_mode(&cur_mode);
        
        if (cur_mode == DYNAMIC_MODE) {
            run_dynamic_mode(sliders_value, simple_expr, last_solution, &quadr_eq, cur_mode);
        } else if (cur_mode == RANDOM_MODE) {
            run_random_mode(&cur_mode);
        } else {
            run_default_mode(buffer_eq, simple_expr, last_solution, &quadr_eq, &now_buf_ind, cur_mode);
        }
    }

    fclose(LOG_F);

    return 0;
}

void update_window_default_mode(char *buffer_eq, char *simple_expr, char *last_solution, struct quadratic_eq *quadr_eq, enum prog_mode cur_mode) {
    ALOGG;
    ASSERT(buffer_eq != NULL, NULL_ERROR);
    ASSERT(simple_expr != NULL, NULL_ERROR);
    ASSERT(last_solution != NULL, NULL_ERROR);
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    txBegin();
    txSetFillColor(TX_BLACK);
    txClear();
    draw_buttons();
    draw_mode_button(cur_mode);
    txDrawText(0, 0, 700, 200, buffer_eq);
    txDrawText(0, 150, 700, 250, simple_expr);
    txDrawText(0, 200, 700, 300, last_solution);
    draw_grafic(quadr_eq);
    
    txEnd();
}

void do_backspace(char *buffer_eq, int *now_buf_ind, int last_sz_el) {
    ALOGG;
    ASSERT(buffer_eq != NULL, NULL_ERROR);
    ASSERT(now_buf_ind != NULL, NULL_ERROR);

    for (int cur_pos = *now_buf_ind - 1; cur_pos >= max(*now_buf_ind - last_sz_el, 0); cur_pos--) {
        buffer_eq[cur_pos] = '\0';
    }

    *now_buf_ind -= last_sz_el;

    if (*now_buf_ind < 0)
        *now_buf_ind = 0;
}

void reset_buffer(char *buffer_eq, int *now_buf_ind) {
    ALOGG;
    ASSERT(buffer_eq != NULL, NULL_ERROR);
    ASSERT(now_buf_ind != NULL, NULL_ERROR);

    free_buffer(buffer_eq);
    *now_buf_ind = 0;
}

void draw_buttons() {
    ALOGG;
    
    txSetFillColor(TX_BLUE);
    txSetColor(TX_WHITE, 3);

    for (size_t but_ind = 0; but_ind < size_of_buttons; but_ind++) {
        draw_button(buttons[but_ind]);
    }
}

char *check_buttons() {
    double mouse_x = txMouseX();
    double mouse_y = txMouseY();

    for (size_t but_ind = 0; but_ind < size_of_buttons; but_ind++) {
        struct button cur_but = buttons[but_ind];

        if (cur_but.x1 <= mouse_x && mouse_x <= cur_but.x2 && cur_but.y1 <= mouse_y && mouse_y <= cur_but.y2) {
            ALOGG;
            return cur_but.but_text;
        }
    }

    return NULL;
}

void draw_grafic(struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(quadr_eq != NULL, NULL_ERROR);
    
    int width_gr = 400, height_gr = 400;
    int sdvig_x = 800, sdvig_y = 150;

    int grafic[width_gr][height_gr] = {0};

    int mid_w = width_gr / 2;
    int mid_h = height_gr / 2;

    for (int y = 0; y < height_gr; y++) {
        grafic[mid_w - 1][y] = 1;
    }

    for (int x = 0; x < width_gr; x++) {
        grafic[x][mid_h + 1] = 1;
    }

    for (double x = 0; x < width_gr; x += 0.1) {

        double y = calc_value_of_quadr_eq(quadr_eq, x - mid_w);
        y = ((int)round(mid_h - y));

        if (0 < y && y < height_gr) {
            grafic[(int)round(x)][(int)y] = 2;
        }
    }

    for (int x = 0; x < width_gr; x++) {
        for (int y = 0; y < height_gr; y++) {

            if (grafic[x][y] == 2 || quadr_eq->n_roots == INF_ROOTS) {
                txSetPixel(x + sdvig_x, y + sdvig_y, TX_YELLOW);
                continue;
            }

            if (grafic[x][y] == 1) {
                txSetPixel(x + sdvig_x, y + sdvig_y, TX_WHITE);
                continue;
            }

            //txSetPixel(x + sdvig_x, y + sdvig_y, TX_BLACK);
        }
    }
}

void draw_mode_button(const enum prog_mode cur_mode) {
    txSetFillColor(TX_GREEN);
    txSetColor(TX_WHITE, 3);
    draw_button(mode_button);

    int cur_num_of_mode = index_of_cur_mode(cur_mode);
    char *name_of_cur_mode = prog_modes[cur_num_of_mode].mode_name;
    txDrawText(mode_button.x2, mode_button.y1, mode_button.x2 + 125, mode_button.y2, name_of_cur_mode);
}

int index_of_cur_mode(const enum prog_mode cur_mode) {
    for (int i = 0; i < size_of_prog_modes; i++) {
        if (prog_modes[i].mode_mode == cur_mode)
            return i;
    }

    return 0;
}

void check_button_mode(enum prog_mode *cur_mode) {
    ALOGG;
    ASSERT(cur_mode != NULL, NULL_ERROR);

    double mouse_x = txMouseX();
    double mouse_y = txMouseY();

    if (mode_button.x1 <= mouse_x && mouse_x <= mode_button.x2 && mode_button.y1 <= mouse_y && mouse_y <= mode_button.y2) {

        int cur_num_of_mode = index_of_cur_mode(*cur_mode);
        cur_num_of_mode = (cur_num_of_mode + 1) % size_of_prog_modes;
        *cur_mode = prog_modes[cur_num_of_mode].mode_mode;

        LOGG("%s\n", prog_modes[cur_num_of_mode].mode_name);
        txSleep(250);
    }
}

void run_default_mode(char *buffer_eq, char *simple_expr, char *last_solution, struct quadratic_eq *pt_quadr_eq, int *pt_now_buf_ind, enum prog_mode cur_mode) {
    ALOGG;
    ASSERT(buffer_eq != NULL, NULL_ERROR);
    ASSERT(simple_expr != NULL, NULL_ERROR);
    ASSERT(last_solution != NULL, NULL_ERROR);
    ASSERT(pt_quadr_eq != NULL, NULL_ERROR);
    ASSERT(pt_now_buf_ind != NULL, NULL_ERROR);

    update_window_default_mode(buffer_eq, simple_expr, last_solution, pt_quadr_eq, cur_mode);
    int last_sz_el = 1;

    char *but_text = check_buttons();

    if (but_text == NULL) return;

    if (strcmp(but_text, "<-") == 0) {
        do_backspace(buffer_eq, pt_now_buf_ind, last_sz_el);

    } else if (strcmp(but_text, "AC") == 0) {
        reset_buffer(buffer_eq, pt_now_buf_ind);
    
    }  else if (strcmp(but_text, "solve") == 0) {

        calc_coefs_of_quadratic_eq(buffer_eq, pt_quadr_eq);
        calc_roots_of_quadratic_eq(pt_quadr_eq);

        free_buffer(last_solution);
        free_buffer(simple_expr);

        sprintf(simple_expr, "You entered: (%lg)x^2 + (%lg)x + (%lg) = 0", pt_quadr_eq->a, pt_quadr_eq->b, pt_quadr_eq->c);

        print_roots_of_quadratic_eq_to_buf(pt_quadr_eq, last_solution);

        update_window_default_mode(buffer_eq, simple_expr, last_solution, pt_quadr_eq, cur_mode);
        txSleep(500);

    } else if (strlen(but_text) != 0 && *pt_now_buf_ind + strlen(but_text) < MAX_LEN_EXPR) {
        if (strcmp(but_text, "x^2") == 0) {
            last_sz_el = 3;
        }

        strcpy(&buffer_eq[*pt_now_buf_ind], but_text);
        *pt_now_buf_ind += last_sz_el;
    }

    update_window_default_mode(buffer_eq, simple_expr, last_solution, pt_quadr_eq, cur_mode);
    txSleep(250);
}

void update_window_dynamic_mode(double sliders_value[], char *simple_expr, char *last_solution, struct quadratic_eq *pt_quadr_eq, enum prog_mode cur_mode) {
    ALOGG;
    ASSERT(sliders_value != NULL, NULL_ERROR);
    ASSERT(simple_expr != NULL, NULL_ERROR);
    ASSERT(last_solution != NULL, NULL_ERROR);
    ASSERT(pt_quadr_eq != NULL, NULL_ERROR);

    txBegin();
    txSetFillColor(TX_BLACK);
    txClear();
    draw_sliders(sliders_value);
    draw_mode_button(cur_mode);
    txDrawText(0, 50, 700, 150, simple_expr);
    txDrawText(0, 100, 700, 200, last_solution);
    draw_grafic(pt_quadr_eq);
    
    txEnd();
}

void run_dynamic_mode(double sliders_value[], char *simple_expr, char *last_solution, struct quadratic_eq *pt_quadr_eq, enum prog_mode cur_mode) {
    ALOGG;
    ASSERT(sliders_value != NULL, NULL_ERROR);
    ASSERT(simple_expr != NULL, NULL_ERROR);
    ASSERT(last_solution != NULL, NULL_ERROR);
    ASSERT(pt_quadr_eq != NULL, NULL_ERROR);

    update_window_dynamic_mode(sliders_value, simple_expr, last_solution, pt_quadr_eq, cur_mode);

    check_sliders(sliders_value);
    set_coefs_for_eq_from_sliders(sliders_value, pt_quadr_eq);
    sprintf(simple_expr, "You entered: (%lg)x^2 + (%lg)x + (%lg) = 0", pt_quadr_eq->a, pt_quadr_eq->b, pt_quadr_eq->c);
    print_roots_of_quadratic_eq_to_buf(pt_quadr_eq, last_solution);

    update_window_dynamic_mode(sliders_value, simple_expr, last_solution, pt_quadr_eq, cur_mode);
    txSleep(100);
}

void draw_button(const struct button but) {
    txRectangle(but.x1, but.y1, but.x2, but.y2);
    txDrawText(but.x1, but.y1, but.x2, but.y2, but.but_text);
}

void draw_slider(const struct slider cur_slider, double value) {
    txSetFillColor(TX_BLUE);
    txSetColor(TX_WHITE, 3);

    txRectangle(cur_slider.x1, cur_slider.y1, cur_slider.x2, cur_slider.y2);
    txDrawText(cur_slider.x1 - 40, cur_slider.y1, cur_slider.x1, cur_slider.y2, cur_slider.slider_text);

    txSetFillColor(TX_RED);
    txSetColor(TX_ORANGE, 2);

    double slider_x_coord = value / (cur_slider.max_value - cur_slider.min_value) * (cur_slider.x2 - cur_slider.x1) + (cur_slider.x1 + cur_slider.x2) / 2;

    txRectangle(slider_x_coord - 1, cur_slider.y1, slider_x_coord + 1, cur_slider.y2);
}

void check_sliders(double sliders_value[]) {
    ALOGG;
    ASSERT(sliders_value != NULL, NULL_ERROR);

    double mouse_x = txMouseX();
    double mouse_y = txMouseY();

    for (size_t slider_i = 0; slider_i < size_of_sliders; slider_i++) {
        struct slider cur_slider = sliders[slider_i];

        if (cur_slider.x1 <= mouse_x && mouse_x <= cur_slider.x2 && cur_slider.y1 <= mouse_y && mouse_y <= cur_slider.y2) {
            ALOGG;
            sliders_value[slider_i] = (cur_slider.max_value - cur_slider.min_value) / (cur_slider.x2 - cur_slider.x1) * (mouse_x - (cur_slider.x1 + cur_slider.x2) / 2);
            LOGG("Slider value: %lg\n", sliders_value[slider_i]);

            return;
        }
    }
}

void draw_sliders(double sliders_value[]) {
    ALOGG;
    ASSERT(sliders_value != NULL, NULL_ERROR);

    for (int i = 0; i < size_of_sliders; i++) {
        draw_slider(sliders[i], sliders_value[i]);
    }
}

void set_coefs_for_eq_from_sliders(double sliders_value[], struct quadratic_eq *pt_quadr_eq) {
    ALOGG;
    ASSERT(sliders_value != NULL, NULL_ERROR);
    ASSERT(pt_quadr_eq != NULL, NULL_ERROR);

    for (int i = 0; i < size_of_sliders; i++) {

        if (strcmp(sliders[i].slider_text, "a") == 0){
            pt_quadr_eq->a = sliders_value[i];
            continue;
        }

        if (strcmp(sliders[i].slider_text, "b") == 0){
            pt_quadr_eq->b = sliders_value[i];
            continue;
        }

        if (strcmp(sliders[i].slider_text, "c") == 0){
            pt_quadr_eq->c = sliders_value[i];
            continue;
        }
    }

    calc_roots_of_quadratic_eq(pt_quadr_eq);
}

void run_random_mode(enum prog_mode *cur_mode) {
    ALOGG;
    struct quadratic_eq quadr_eq = {0};
    enum rand_status now_status = RAND_COEFS;
    double y_up = 100;
    double y_down = 400;

    struct button rand_values[] = {
        {100, 100, 200, 150, " "},
        {200, 150, 300, 200, " "},
        {300, 200, 400, 250, " "},

        {100, 250, 200, 300, " "},
        {200, 300, 300, 350, " "},
        {300, 350, 400, 400, " "}
    };

    size_t rand_values_size = sizeof(rand_values) / sizeof(rand_values[0]);

    double speed = 2;
    double lot_x1 = -200;
    double lot_x2 = -200;
    int win_lots = 0;

    int fps_delay = 20;
    int default_lot_delay = 40;
    double bill = 20;
    int payed = 0;
    int price = 10;
    double lot_diapason = 5;

    while (1) {
        if (txMouseButtons() == LEFT_CLICK) {
            check_button_mode(cur_mode);

            if (*cur_mode != RANDOM_MODE)
                return;
            
            check_random_buttons(&now_status);
        }

        if (now_status == RAND_RESTART) {
            if (win_lots == 2) {
                default_lot_delay = max(default_lot_delay - 10, 1);
                lot_diapason = max(lot_diapason - 0.5, 2.5);
            }
            
            now_status = RAND_COEFS;
            speed = 2;
            lot_x1 = -200;
            lot_x2 = -200;
            win_lots = 0;
            fps_delay = 20;
            payed = 0;
        }

        if (now_status == RAND_COEFS) {
            update_rand_values(&quadr_eq, rand_values, rand_values_size, y_up, y_down);

        } else if (now_status == RAND_X) {
            if (payed == 0) {
                if (bill + 1e-5 < price) {
                    now_status = RAND_COEFS;
                    continue;
                }

                bill -= price;
                payed = 1;
            }

            update_lot_x(&lot_x1, &speed);
            fps_delay = default_lot_delay;

        } else if (now_status == RAND_X2) {
            update_lot_x(&lot_x2, &speed);
            fps_delay = default_lot_delay;

        } else if (now_status == RAND_WAIT) {
            calc_roots_of_quadratic_eq(&quadr_eq);

            if (check_lot_x_diapason(quadr_eq.x1, lot_x1, lot_diapason))
                win_lots++;
            if (check_lot_x_diapason(quadr_eq.x2, lot_x2, lot_diapason))
                win_lots++;
            if (quadr_eq.n_roots != TWO_ROOTS)
                win_lots = 0;
            
            fps_delay = 20;

            if (win_lots == 2) {
                now_status = RAND_WIN;
                bill += 100;
            } else {
                now_status = RAND_LOSE;
            }
        } else if (now_status == RAND_WIN) {
            ;
        } else if (now_status == RAND_LOSE) {
            ;
        }
        calc_roots_of_quadratic_eq(&quadr_eq);
        update_window_random_mode(&quadr_eq, *cur_mode, rand_values, rand_values_size, lot_x1, lot_x2, bill);

        txSleep(fps_delay);
    }
}



void update_window_random_mode(struct quadratic_eq *pt_quadr_eq, enum prog_mode cur_mode, struct button rand_values[], size_t rand_values_size, double lot_x1, double lot_x2, double bill) {
    ALOGG;
    ASSERT(pt_quadr_eq != NULL, NULL_ERROR);

    txBegin();
    txSetFillColor(TX_BLACK);
    txClear();
    draw_rand_values(rand_values, rand_values_size);
    draw_random_buttons();
    draw_mode_button(cur_mode);
    draw_grafic(pt_quadr_eq);
    draw_lot_x(lot_x1);
    draw_lot_x(lot_x2);
    draw_bill(bill);
    
    txEnd();
}

void check_random_buttons(enum rand_status *now_status) {
    double mouse_x = txMouseX();
    double mouse_y = txMouseY();

    for (int i = 0; i < size_of_rand_buttons; i++) {
        struct button cur_but = rand_buttons[i];
        if (cur_but.x1 <= mouse_x && mouse_x <= cur_but.x2 && cur_but.y1 <= mouse_y && mouse_y <= cur_but.y2) {
            if (strcmp(cur_but.but_text, "STOP COEFS") == 0) {
                if (*now_status == RAND_COEFS)
                    *now_status = RAND_X;
                return;
            }
            if (strcmp(cur_but.but_text, "STOP X") == 0) {
                if (*now_status == RAND_X)
                    *now_status = RAND_X2;
                else
                    *now_status = RAND_WAIT;
                txSleep(300);
                return;
            }
            if (strcmp(cur_but.but_text, "RESTART") == 0) {
                *now_status = RAND_RESTART;
                return;
            }
        }
    }
}

void draw_random_buttons() {
    txSetFillColor(TX_RED);
    txSetColor(TX_WHITE, 3);
    for (int i = 0; i < size_of_rand_buttons; i++) {
        draw_button(rand_buttons[i]);
    }
}

void draw_rand_values(struct button rand_values[], size_t rand_values_size) {
    ALOGG;

    txSetColor(TX_WHITE, 3);

    for (size_t val_ind = 0; val_ind < rand_values_size; val_ind++) {
        if (val_ind == 3) txSetColor(TX_YELLOW, 3);
        draw_text(rand_values[val_ind]);
    }
}

void update_rand_values(struct quadratic_eq *pt_quadr_eq, struct button rand_values[], size_t rand_values_size, double y_up, double y_down) {
    ALOGG;
    double speed = 40;

    for (size_t val_ind = 0; val_ind < rand_values_size; val_ind++) {

        rand_values[val_ind].y1 -= speed;
        rand_values[val_ind].y2 -= speed;

        if ((rand_values[val_ind].y1 + rand_values[val_ind].y2) / 2 < y_up) {
            double move_down = y_down - y_up;
            double new_value = 0;

            rand_values[val_ind].y1 += move_down;
            rand_values[val_ind].y2 += move_down;

            switch (val_ind % 3)
            {
            case 0:
                new_value = (double)(rand() % 201 - 100) / 200.0;
                pt_quadr_eq->a = new_value;
                break;
            
            case 1:
                new_value = (double)(rand() % 101 - 50) / 7.0;
                pt_quadr_eq->b = new_value;
                break;
            
            case 2:
                new_value = (double)(rand() % 201 - 100) / 2.0;
                pt_quadr_eq->c = new_value;
                break;
            
            default:
                break;
            }

            sprintf(rand_values[val_ind].but_text, "%.2lg", new_value);
        }
    }
}

void draw_text(const struct button but) {
    txDrawText(but.x1, but.y1, but.x2, but.y2, but.but_text);
}

void draw_lot_x(double x) {
    txSetColor(TX_RED, 4);
    txSetFillColor(TX_RED);
    txRectangle(1000 + x - 1, 349 - 1, 1000 + x + 1, 349 + 1);
}

void update_lot_x(double *lot_x, double *speed) {
    *lot_x += *speed;
    if (*lot_x > 200 || *lot_x < -200)
        *speed *= -1;
}

void draw_bill(double bill) {
    txSetColor(TX_YELLOW, 4);
    txSetFillColor(TX_BLACK);

    struct button but_bill = {900, 0, 1200, 100};
    sprintf(but_bill.but_text, "You have %.2lf$ on your bill", bill);
    draw_text(but_bill);
}

int check_lot_x_diapason(double lot_win, double lot_x, double lot_diapason) {
    return (lot_win - lot_diapason < lot_x && lot_x < lot_win + lot_diapason);
}