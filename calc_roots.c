#include "calc_roots.h"

void calc_root_of_linear(struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    ASSERT(check_diapason_error_for_double(quadr_eq->b) != 0, ERROR_OF_DIAPASON_OF_DOUBLE);
    ASSERT(check_diapason_error_for_double(quadr_eq->c) != 0, ERROR_OF_DIAPASON_OF_DOUBLE);

    if (equal(quadr_eq->b, 0) == 1) {
        if (equal(quadr_eq->c, 0) == 1) {
            quadr_eq->n_roots = INF_ROOTS;

            return;
        }
        quadr_eq->n_roots = NO_ROOTS;

        return;
    }

    quadr_eq->n_roots = ONE_ROOT;
    quadr_eq->x1 = -quadr_eq->c / quadr_eq->b;

    return;
}

void calc_roots_of_quadratic_eq(struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    struct quadratic_eq quadr_eq_no_ptr = *quadr_eq;

    ASSERT(check_diapason_error_for_double(quadr_eq_no_ptr.a) != 0, ERROR_OF_DIAPASON_OF_DOUBLE);
    ASSERT(check_diapason_error_for_double(quadr_eq_no_ptr.b) != 0, ERROR_OF_DIAPASON_OF_DOUBLE);
    ASSERT(check_diapason_error_for_double(quadr_eq_no_ptr.c) != 0, ERROR_OF_DIAPASON_OF_DOUBLE);

    if (equal(quadr_eq_no_ptr.a, 0) == 1) { //линейное ур-е
        calc_root_of_linear(quadr_eq);

        return;
    }

    quadr_eq->discriminant = my_pow(quadr_eq_no_ptr.b, 2) - 4 * quadr_eq_no_ptr.a * quadr_eq_no_ptr.c; //квадратное уравнение
    quadr_eq->discriminant = flush_to_zero(quadr_eq->discriminant);

    if (equal(quadr_eq->discriminant, 0) == 1) {
        quadr_eq->x1 = quadr_eq->x2 = -quadr_eq_no_ptr.b / (2 * quadr_eq_no_ptr.a);
        quadr_eq->n_roots = ONE_ROOT;

        return;
    } else if (quadr_eq->discriminant < 0) {
        quadr_eq->n_roots = NO_ROOTS;

        return;
    }

    double sqrt_discriminant = sqrt(quadr_eq->discriminant);

    quadr_eq->x1 = (-quadr_eq_no_ptr.b + sqrt_discriminant) / (2 * quadr_eq_no_ptr.a);
    quadr_eq->x2 = (-quadr_eq_no_ptr.b - sqrt_discriminant) / (2 * quadr_eq_no_ptr.a);

    if (quadr_eq->x1 > quadr_eq->x2) {
        SWAP(quadr_eq->x2, quadr_eq->x1, double)
    }

    quadr_eq->n_roots = TWO_ROOTS;

    return;
}