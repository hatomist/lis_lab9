#include "check.h"

double
get_double (char *prompt, bool non_negative, bool cant_be_zero) {
    char number_str[81];
    char *end_ptr;  // pointer to the first not convertible to double symbol
    double number;
    printf ("%s", prompt);

    // read string, check if it can be converted to double, retry if failed
    while (true) {
        scanf ("%80s", number_str);

        // drop stdin buffer
        int c;
        while((c = getchar()) != '\n' && c != EOF);

        number = strtod (number_str, &end_ptr);
        if ((number < 0 && non_negative) || errno == ERANGE || *end_ptr != '\0'
        || errno == EINVAL || (cant_be_zero && number == 0)) {
            printf ("Ввод не является числом или число не входит в допустимый диапазон, попробуйте ещё раз\n");
            errno = 0;
        }
        else
            return number;
    }
}

long long
get_ll (char *prompt) {
    char number_str[81];
    char *end_ptr;  // pointer to the first not convertible to integer symbol
    long long number;
    printf ("%s", prompt);

    // read string, check if it can be converted to long long, retry if failed
    while (true) {
        scanf ("%80s", number_str);

        // drop stdin buffer
        int c;
        while((c = getchar()) != '\n' && c != EOF);

        number = strtoll (number_str, &end_ptr, 10);
        if (errno == ERANGE || *end_ptr != '\0') {
            printf ("Ввод не является числом или число не входит в допустимый диапазон, попробуйте ещё раз\n");
            errno = 0;
        }
        else
        return number;
    }
}

unsigned long long
get_ull (char *prompt) {
    char number_str[81];
    char *end_ptr;  // pointer to the first not convertible to integer symbol
    unsigned long long number;
    printf ("%s", prompt);

    // read string, check if it can be converted to long long, retry if failed
    while (true) {
        scanf ("%80s", number_str);

        // drop stdin buffer
        int c;
        while((c = getchar()) != '\n' && c != EOF);

        number = strtoull (number_str, &end_ptr, 10);
        if (errno == ERANGE || *end_ptr != '\0' || errno == ERANGE) {
            printf ("Ввод не является числом или число не входит в допустимый диапазон, попробуйте ещё раз\n");
            errno = 0;
        }
        else
            return number;
    }
}
