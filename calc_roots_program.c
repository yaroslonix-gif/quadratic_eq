#include "calc_roots_program.h"

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

size_t size_of_buttons = sizeof(buttons) / sizeof(buttons[0]);


int main() {
    LOG_F = fopen("log.txt", "w");
    ASSERT(LOG_F != NULL, NULL_ERROR);
    ALOGG;
    
    test();

    struct quadratic_eq quadr_eq = {0};

    txCreateWindow(WINDOW_WIDTH, 600);
    txTextCursor(0);
    txSetFillColor(TX_BLACK);
    txClear();
    draw_buttons();

    char buffer_eq[MAX_LEN_EXPR] = {0};
    char simple_expr[MAX_LEN_EXPR] = {0};
    char last_solution[MAX_LEN_EXPR] = {0};
    int now_buf_ind = 0;
    int last_sz_el = 1;

    while (1) {

        while (txMouseButtons() != LEFT_CLICK)
            txSleep(10);
        
        char *but_text = check_buttons();

        if (but_text == NULL) continue;

        if (strcmp(but_text, "<-") == 0) {
            do_backspace(buffer_eq, &now_buf_ind, last_sz_el);

        } else if (strcmp(but_text, "AC") == 0) {
            reset_buffer(buffer_eq, &now_buf_ind);
        
        }  else if (strcmp(but_text, "solve") == 0) {

            calc_coefs_of_quadratic_eq(buffer_eq, &quadr_eq);
            calc_roots_of_quadratic_eq(&quadr_eq);

            free_buffer(last_solution);
            free_buffer(simple_expr);

            sprintf(simple_expr, "You entered: (%lg)x^2 + (%lg)x + (%lg) = 0", quadr_eq.a, quadr_eq.b, quadr_eq.c); //

            print_roots_of_quadratic_eq_to_buf(&quadr_eq, last_solution);

            update_window(buffer_eq, simple_expr, last_solution, &quadr_eq);
            txSleep(500);

        } else if (strlen(but_text) != 0 && now_buf_ind + strlen(but_text) < MAX_LEN_EXPR) {
            if (strcmp(but_text, "x^2") == 0) {
                last_sz_el = 3;
            }

            strcpy(&buffer_eq[now_buf_ind], but_text);
            now_buf_ind += last_sz_el;
        }

        update_window(buffer_eq, simple_expr, last_solution, &quadr_eq);
        txSleep(300);

        last_sz_el = 1;
    }

    fclose(LOG_F);

    return 0;
}

void update_window(char *buffer_eq, char *simple_expr, char *last_solution, struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(buffer_eq != NULL, NULL_ERROR);
    ASSERT(simple_expr != NULL, NULL_ERROR);
    ASSERT(last_solution != NULL, NULL_ERROR);
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    txBegin();
    txSetFillColor(TX_BLACK);
    txClear();
    draw_buttons();
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
        struct button cur_but = buttons[but_ind];
        
        txRectangle(cur_but.x1, cur_but.y1, cur_but.x2, cur_but.y2);
        txDrawText(cur_but.x1, cur_but.y1, cur_but.x2, cur_but.y2, cur_but.but_text);
    }
}

char *check_buttons() {
    for (size_t but_ind = 0; but_ind < size_of_buttons; but_ind++) {
        struct button cur_but = buttons[but_ind];
        double mouse_x = txMouseX();
        double mouse_y = txMouseY();

        if (cur_but.x1 <= mouse_x && mouse_x <= cur_but.x2 && cur_but.y1 <= mouse_y && mouse_y <= cur_but.y2) {
            ALOGG;
            return cur_but.but_text;
        }
    }

    return NULL;
}

void draw_grafic(struct quadratic_eq *quadr_eq) {
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