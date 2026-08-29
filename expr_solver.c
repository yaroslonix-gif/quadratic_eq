#include "expr_solver.h"

void calc_coefs_of_quadratic_eq(char *eq, struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(eq != NULL, NULL_ERROR);
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    char left_eq[MAX_LEN_EXPR] = {0};
    char right_eq[MAX_LEN_EXPR] = {0};

    parsing_by_equal(eq, left_eq, right_eq);

    // printf("%s\n", left_eq);
    // printf("%s\n", right_eq);

    int left_a = 0, left_b = 0, left_c = 0;

    calc_coefs_from_expr(left_eq, &left_a, &left_b, &left_c);

    // printf("%d\n", left_a);
    // printf("%d\n", left_b);
    // printf("%d\n", left_c);

    int right_a = 0, right_b = 0, right_c = 0;

    calc_coefs_from_expr(right_eq, &right_a, &right_b, &right_c);
    
    // printf("%d\n", right_a);
    // printf("%d\n", right_b);
    // printf("%d\n", right_c);

    quadr_eq->a = left_a - right_a;
    quadr_eq->b = left_b - right_b;
    quadr_eq->c = left_c - right_c;
}

void parsing_by_equal(char eq[], char left_eq[], char right_eq[]) {
    ALOGG;
    ASSERT(eq != NULL, NULL_ERROR);
    ASSERT(left_eq != NULL, NULL_ERROR);
    ASSERT(right_eq != NULL, NULL_ERROR);

    int index_of_equal = 0;

    for (int num_ch = 0; num_ch < strlen(eq); num_ch++) {
        if (eq[num_ch] == '=') {
            index_of_equal = num_ch;
            eq[num_ch] = '\0';
            break;
        }
    }

    strcpy(left_eq, eq);
    strcpy(right_eq, &eq[index_of_equal + 1]);
    eq[index_of_equal] = '=';
}

void calc_coefs_from_expr(char eq[], int *a, int *b, int *c) {
    ALOGG;
    ASSERT(eq != NULL, NULL_ERROR);
    ASSERT(a != NULL, NULL_ERROR);
    ASSERT(b != NULL, NULL_ERROR);
    ASSERT(c != NULL, NULL_ERROR);

    destroy_multiply_x(eq);

    int size_of_eq = strlen(eq);

    char coefs_x2[MAX_LEN_EXPR] = {0};
    char coefs_x1[MAX_LEN_EXPR] = {0};
    char numbers[MAX_LEN_EXPR] = {0};

    char cur_value[MAX_LEN_EXPR] = {0};
    int cur_i = 0;

    for (int i_eq = 0; i_eq < size_of_eq; i_eq++) {
        if (eq[i_eq] == ' ')
            continue;
        
        if (eq[i_eq] != 'x') { // сброс буфера в число, если не относится к х (2*2*2+...)
            if (isdigit(eq[i_eq]) == 0 && (eq[i_eq] == '-' || eq[i_eq] == '+')) {
                add_str_s2_to_s1(numbers, cur_value);
                cur_i = 0;
                free_buffer(cur_value);
            }
            cur_value[cur_i] = eq[i_eq];
            cur_i++;

            continue;
        }
//          check max len                  check pow          check pow(x, 2)
        if (i_eq + 2 < size_of_eq && eq[i_eq + 1] == '^' && eq[i_eq + 2] == '2') { // parsing ax^2
            push_1_after_add_or_sub_if_only_sign_in_buf(cur_value);

            add_str_s2_to_s1(coefs_x2, cur_value);
            cur_i = 0;
            i_eq += 2;
            free_buffer(cur_value);

            continue;
        }
//          check max len             check pow                   check end of buf
        if ((i_eq + 1 < size_of_eq && eq[i_eq + 1] != '^') || (i_eq + 1 == size_of_eq)) { // parsing ax
            push_1_after_add_or_sub_if_only_sign_in_buf(cur_value);

            add_str_s2_to_s1(coefs_x1, cur_value);
            cur_i = 0;
            free_buffer(cur_value);

            continue;
        }
    }

    add_str_s2_to_s1(numbers, cur_value);

    *a = solve_expr(coefs_x2);
    *b = solve_expr(coefs_x1);
    *c = solve_expr(numbers);

    printf("%s\n", coefs_x2);
    printf("%s\n", coefs_x1);
    printf("%s\n", numbers);
}

void push_1_after_add_or_sub_if_only_sign_in_buf(char *cur_buf) {
    ALOGG;
    ASSERT(cur_buf != NULL, NULL_ERROR);

    if (strlen(cur_buf) == 1 && (cur_buf[0] == '+' || cur_buf[0] == '-'))
        cur_buf[1] = '1';
    else if (strlen(cur_buf) == 0)
        cur_buf[0] = '1';
}

int solve_expr(char *expr) {
    ALOGG;
    ASSERT(expr != NULL, NULL_ERROR);

    int size_of_expr = strlen(expr);
    struct node expr_n[size_of_expr] = {};

    for (int num_of_node = 0; num_of_node < size_of_expr; num_of_node++) {
        if (isdigit(expr[num_of_node]) == 1) {

            struct node node_el = {};
            node_el.type_el = NUMBER;
            node_el.value_el.number = convert_to_digit(expr[num_of_node]);
            node_el.name_el = expr[num_of_node];

            expr_n[num_of_node] = node_el;

            continue;
        }

        // if (isalpha(expr[num_of_node]) == 1) {
        //     struct node node_el = {
        //         .type_el = VARIA,
        //         .value_el.varia = expr[num_of_node],
        //         .name_el = expr[num_of_node]
        //     };
        //     expr_n[num_of_node] = node_el;

        //     continue;
        // }

        struct node node_el = {};
        node_el.type_el = OPERATOR;
        node_el.value_el.opera = convert_to_op(expr[num_of_node]);
        node_el.name_el = expr[num_of_node];

        expr_n[num_of_node] = node_el;
    }

    int ans = calc(expr_n, 0, size_of_expr);

    return ans;
}

int find_low_op(struct node expr_n[], int left, int right) {
    ALOGG;
    ASSERT(expr_n != NULL, NULL_ERROR);

    int min_op = 100;
    int min_op_ind = -1;

    for (int num_ch = left; num_ch < right; num_ch++) {
        if (expr_n[num_ch].type_el == OPERATOR) {
            if (min_op >= get_prioritet(expr_n[num_ch])) {
                min_op = get_prioritet(expr_n[num_ch]);
                min_op_ind = num_ch;
            }
        }
    }

    return min_op_ind;
}

int calc(struct node expr_n[], int left, int right) {
    ALOGG;
    ASSERT(expr_n != NULL, NULL_ERROR);

    if (right - left == 0) {
        return 0;
    }

    if (right - left == 1) {
        return expr_n[left].value_el.number;
    }

    int low_op = find_low_op(expr_n, left, right);
    if (low_op == -1) {
        return convert_nodes_to_number(expr_n, left, right);
    }

    int left_value = calc(expr_n, left, low_op);
    int right_value = calc(expr_n, low_op + 1, right);
    
    OPERATION(expr_n[low_op].value_el.opera, left_value, right_value);
}

enum op convert_to_op(char ch) {
    ALOGG;
    for (size_t num_cur_op = 0; num_cur_op < sz_conv_operators; num_cur_op++) {
        if (ch == conv_operators[num_cur_op].opera_ch)
            return conv_operators[num_cur_op].opera_op;
    }

    return ADD;
}

int convert_to_digit(char ch) {
    ALOGG;
    return (ch - '0');
}

int convert_nodes_to_number(struct node expr_n[], int left, int right) {
    ALOGG;
    ASSERT(expr_n != NULL, NULL_ERROR);

    int num = 0;
    for (int num_of_node = left; num_of_node < right; num_of_node++) {
        if (expr_n[num_of_node].type_el == NUMBER)
            num = num * 10 + expr_n[num_of_node].value_el.number;
    }

    return num;
}

void add_str_s2_to_s1(char *s1, char *s2) {
    ALOGG;
    ASSERT(s1 != NULL, NULL_ERROR);
    ASSERT(s2 != NULL, NULL_ERROR);

    int sz_s1 = strlen(s1);
    strcpy(&s1[sz_s1], s2);

    return;
}

void free_buffer(char *buffer) {
    ALOGG;
    ASSERT(buffer != NULL, NULL_ERROR);

    for (int i_buf = 0; i_buf < MAX_LEN_EXPR; i_buf++) {
        buffer[i_buf] = 0;
    }
}

void destroy_multiply_x(char *expr) {
    ALOGG;
    ASSERT(expr != NULL, NULL_ERROR);

    char new_expr[MAX_LEN_EXPR] = {0};
    int num_new_expr = 0;

    for (int num_ch = 0; num_ch < MAX_LEN_EXPR; num_ch++) {
        if (expr[num_ch] == '*') {
            if (num_ch + 1 < MAX_LEN_EXPR && expr[num_ch + 1] == 'x') {
                continue;
            }
        }
        new_expr[num_new_expr] = expr[num_ch];
        num_new_expr++;
    }

    strcpy(expr, new_expr);
}

void return_multiply_x(char *expr) {
    ALOGG;
    ASSERT(expr != NULL, NULL_ERROR);

    char new_expr[MAX_LEN_EXPR] = {0};
    int num_new_expr = 0;
    int size_expr = strlen(expr);

    for (int num_ch = 0; num_ch < size_expr; num_ch++) {
        if (expr[num_ch] == 'x') {
            if (num_ch - 1 >= 0 && isdigit(expr[num_ch - 1]) == 1) {
                new_expr[num_new_expr] = '*';
                num_new_expr++;
            }
        }

        new_expr[num_new_expr] = expr[num_ch];
        num_new_expr++;
    }

    strcpy(expr, new_expr);
}

int get_prioritet(struct node element) {
    ALOGG;
    for (size_t i = 0; i < sz_conv_operators; i++) {
        if (element.name_el == conv_operators[i].opera_ch)
            return conv_operators[i].prioritet;
    }

    return 0;
}