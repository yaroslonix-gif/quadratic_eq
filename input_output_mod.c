#include "input_output_mod.h"

void skip_entered_data(FILE *fpin, int push_n) {
    ALOGG;
    ASSERT(fpin != NULL, NULL_ERROR);

    while (getc(fpin) != '\n') {
        continue;
    }

    if (push_n == 1) {
        ungetc('\n', fpin);
    }
}

void print_roots_of_quadratic_eq(const struct quadratic_eq *quadr_eq, FILE *fpout) {
    ALOGG;
    ASSERT(quadr_eq != NULL, NULL_ERROR);
    ASSERT(fpout != NULL, NULL_ERROR);

    //print_line();
    switch (quadr_eq->n_roots)
    {
        case NO_ROOTS:
            fprintf(fpout, "No roots\n");
            break;

        case ONE_ROOT:
            fprintf(fpout, "1 root: %.2lf\n", quadr_eq->x1);
            break;

        case TWO_ROOTS:
            fprintf(fpout, "2 roots: %.2lf and %.2lf\n", quadr_eq->x1, quadr_eq->x2);
            break;

        case INF_ROOTS:
            fprintf(fpout, "Too Many Roots\n");
            break;

        default:
            break;
    }
    //print_line();
}

void print_roots_of_quadratic_eq_to_buf(const struct quadratic_eq *quadr_eq, char *buf_out) {
    ALOGG;
    ASSERT(quadr_eq != NULL, NULL_ERROR);
    ASSERT(buf_out != NULL, NULL_ERROR);

    switch (quadr_eq->n_roots)
    {
        case NO_ROOTS:
            sprintf(buf_out, "Quadratic eq has no roots");
            break;

        case ONE_ROOT:
            sprintf(buf_out, "Quadratic eq has 1 root: %.2lf", quadr_eq->x1);
            break;

        case TWO_ROOTS:
            sprintf(buf_out, "Quadratic eq has 2 roots: %.2lf and %.2lf", quadr_eq->x1, quadr_eq->x2);
            break;

        case INF_ROOTS:
            sprintf(buf_out, "Quadratic eq has Too Many Roots");
            break;

        default:
            break;
    };
}


void set_input_and_output(enum input_output_mode *mode, FILE **fp_quadr_eq_in, FILE **fp_quadr_eq_out, int argc, char *argv[]) {
    ALOGG;
    ASSERT(mode != NULL, NULL_ERROR);
    ASSERT(fp_quadr_eq_in != NULL, NULL_ERROR);
    ASSERT(fp_quadr_eq_out != NULL, NULL_ERROR);
    ASSERT(argv != NULL, NULL_ERROR);

    if (argc == 2) {
        if (strcmp(argv[1], "user_input") == 0) {
            *fp_quadr_eq_in = stdin;
            *fp_quadr_eq_out = stdout;
            *mode = USER_INPUT_OUTPUT;
        } else {
            *mode = DEFAULT_INPUT_OUTPUT;

            PRINT_ERR("Error in params; Using default values\n");
        }
    } else if (argc == 4) {

        *fp_quadr_eq_in = fopen(argv[2], "r");
        *fp_quadr_eq_out = fopen(argv[3], "w");

        if (strcmp(argv[1], "file_input") == 0 && *fp_quadr_eq_in != NULL && *fp_quadr_eq_out != NULL) {
            *mode = FILE_INPUT_OUTPUT;
        } else {

            *mode = DEFAULT_INPUT_OUTPUT;

            if (*fp_quadr_eq_in == NULL && *fp_quadr_eq_out == NULL)
                PRINT_ERR("Error in params; Can't find files; Using default values\n")
            else if (*fp_quadr_eq_in == NULL)
                PRINT_ERR("Error in params; Can't find file %s; Using default values\n", argv[2])
            else if (*fp_quadr_eq_out == NULL)
                PRINT_ERR("Error in params; Can't find file %s; Using default values\n", argv[3])
            else
                PRINT_ERR("Error in params; Using default values\n")
        }
    } else if (argc != 1) {
        PRINT_ERR("Unknown error in params\n")
    }

    print_line();

    if (*mode == DEFAULT_INPUT_OUTPUT) {

        printf("Using default mode\n");
        printf("Standard input and output\n");
        PRINT_COLOR(GREEN, "Enter 3 double numbers (coefficients of quadratic equation) a, b, c (example: 1.0 2.0 3.6):\n")

    } else if (*mode == USER_INPUT_OUTPUT) {

        printf("Using user_input mode\n");
        printf("Standard input and output\n");
        PRINT_COLOR(GREEN, "Enter 3 double numbers (coefficients of quadratic equation) a, b, c (example: 1.0 2.0 3.6):\n")

    } else if (*mode == FILE_INPUT_OUTPUT) {

        printf("Using file_input mode\n");
        printf("File input = %s and output = %s\n", argv[2], argv[3]);

    }

    print_line();
}

void close_input_and_output(FILE *fp_quadr_eq_in, FILE *fp_quadr_eq_out) {
    ALOGG;
    ASSERT(fp_quadr_eq_in != NULL, NULL_ERROR);
    ASSERT(fp_quadr_eq_out != NULL, NULL_ERROR);

    if (fclose(fp_quadr_eq_in) != 0)
        PRINT_ERR("Error in closing input_file\n")

    if (fclose(fp_quadr_eq_out) != 0)
        PRINT_ERR("Error in closing output_file\n")
}

void print_line(void) {
    ALOGG;
    printf(YELLOW);
    for (int i = 0; i < LINE_LEN; i++) {
        printf("=");
    }
    printf("%s\n", RESET_COLOR);
}

int check_buf_for_valid(char *buf) {
    ALOGG;
    ASSERT(buf != NULL, NULL_ERROR);

    while (*buf != '\0') {
        if (isdigit(*buf) != 1 && *buf != '.' && *buf != ' ' && *buf != '-'  && *buf != '+' && *buf != '\n')
            return 0;
        buf++;
    }

    return 1;
}

void print_quadratic_eq(struct quadratic_eq quadr_eq) {
    ALOGG;
    int output_coefs = 3;

    if (equal(quadr_eq.a, 1)) {
        printf("x^2");
    } else if (equal(quadr_eq.a, 0) == 0) {
        printf("%lgx^2", quadr_eq.a);
    } else {
        output_coefs--;
    }

    if (equal(fabs(quadr_eq.b), 1) == 1) {
        if (output_coefs == 3) {
            if (quadr_eq.b > 0)
                printf(" + ");
            else
                printf(" - ");
        } else {
            if (quadr_eq.b < 0)
                printf("-");
        }
        printf("x");
    } else if (equal(quadr_eq.b, 0) == 0) {
        if (output_coefs == 3) {
            if (quadr_eq.b > 0)
                printf(" + ");
            else
                printf(" - ");
        } else {
            if (quadr_eq.b < 0)
                printf("-");
        }
        printf("%lgx", fabs(quadr_eq.b));
    }  else {
        output_coefs--;
    }
    
    
    if (equal(quadr_eq.c, 0) == 1) {
        if (output_coefs == 1) {
            printf("0");
        }
    } else {
        if (output_coefs != 1) {
            if (quadr_eq.c > 0)
                printf(" + ");
            else
                printf(" - ");
        }
        printf("%lg", fabs(quadr_eq.c));
    }

    printf(" = 0\n");

}

int scan_sq_eq(char buf[], struct quadratic_eq *quadr_eq) {
    ALOGG;
    ASSERT(buf != NULL, NULL_ERROR);
    ASSERT(quadr_eq != NULL, NULL_ERROR);

    int n_scaned = sscanf(buf, "%lf%*c%*c%*c%lf%*c%lf", &quadr_eq->a, &quadr_eq->b, &quadr_eq->c);
    return n_scaned;
}