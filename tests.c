#include "tests.h"

int run_one_test_with_no_random_coef(const struct quadratic_eq *good_struct, size_t n_test) {
    ALOGG;
    ASSERT(good_struct != NULL, NULL_ERROR);

    struct quadratic_eq test_struct = *good_struct;

    calc_roots_of_quadratic_eq(&test_struct);

    if (test_struct.n_roots != good_struct->n_roots || !equal(test_struct.x1, good_struct->x1) || !equal(test_struct.x2, good_struct->x2)) {
        print_line();
        LOGG("Test %zu FAILED: a = %lg, b = %lg, c = %lg\n"
               "expected %2d roots, expected x1 = %6.2lf, expected x2 = %6.2lf,\n"
               "out      %2d roots, expected x1 = %6.2lf, expected x2 = %6.2lf.\n"
            , n_test, good_struct->a, good_struct->b, good_struct->c, good_struct->n_roots, good_struct->x1, good_struct->x2,
            test_struct.n_roots, test_struct.x1, test_struct.x2);

        return 1;
    }

    return 0;
}

void test(void) {
    ALOGG;
    int failed_tests = 0;

    // test function calc_roots_of_quadratic_eq
    static struct quadratic_eq good_structs[] = {
        {.a = 0, .b = 0, .c = 0, .n_roots = INF_ROOTS, .x1 = 0, .x2 = 0},
        {.a = 0, .b = 0, .c = 1, .n_roots = NO_ROOTS, .x1 = 0, .x2 = 0},
        {.a = 1, .b = 2, .c = 3, .n_roots = NO_ROOTS, .x1 = 0, .x2 = 0},
        {.a = 0, .b = 1, .c = 1, .n_roots = ONE_ROOT, .x1 = -1, .x2 = 0},
        {.a = 1, .b = 2, .c = 1, .n_roots = ONE_ROOT, .x1 = -1, .x2 = -1},
        {.a = 1, .b = -12, .c = 35, .n_roots = TWO_ROOTS, .x1 = 5, .x2 = 7},
    };

    size_t len_of_good_structs = sizeof(good_structs) / sizeof(good_structs[0]);

    for (size_t n_test = 0; n_test < len_of_good_structs; n_test++) {
        failed_tests += run_one_test_with_no_random_coef(&good_structs[n_test], n_test);
    }

    for (int n_test = 0; n_test < 10; n_test++) {
        failed_tests += run_one_test_with_random_coef();
    }

    // test function my_pow
    if (equal(my_pow(100.1, 0), 1) == 0) {
        LOGG("Test: pow(100.1, 0); Error in result %lf != 1\n", my_pow(100.1,0));
        failed_tests++;
    }

    if (equal(my_pow(-2, 2), 4) == 0) {
        LOGG("Test: pow(-2, 2); Error in result %lf != 4\n", my_pow(-2,2));
        failed_tests++;
    }

    if (failed_tests == 0) {
        LOGG("All is ok!\n");
    }
}

int run_one_test_with_random_coef() {
    ALOGG;
    struct quadratic_eq test_struct = {.a = (double)(rand() % 2000001 - 1000000) / (double)100.0,
                                       .b = (double)(rand() % 2000001 - 1000000) / (double)100.0,
                                       .c = (double)(rand() % 2000001 - 1000000) / (double)100.0,
    };

    calc_roots_of_quadratic_eq(&test_struct);

    if (test_struct.n_roots == 1 && equal(test_struct.a * my_pow(test_struct.x1, 2) + test_struct.b * test_struct.x1 + test_struct.c, 0) == 0) {

        LOGG("Error in test: a = %lf, b = %lf, c = %lf. Out: n_roots = %d, x1 = %lf\n", test_struct.a, test_struct.b, test_struct.c,
            test_struct.n_roots, test_struct.x1);

        return 1;
    }

    if (test_struct.n_roots == 2 && (equal(test_struct.a * my_pow(test_struct.x1, 2) + test_struct.b * test_struct.x1 + test_struct.c, 0) == 0 ||
        equal(test_struct.a * my_pow(test_struct.x2, 2) + test_struct.b * test_struct.x2 + test_struct.c, 0) == 0)) {
            
        LOGG("Error in test: a = %lf, b = %lf, c = %lf. Out: n_roots = %d, x1 = %lf, x2 = %lf\n", test_struct.a, test_struct.b, test_struct.c,
            test_struct.n_roots, test_struct.x1, test_struct.x2);

        return 1;
    }

    return 0;
}