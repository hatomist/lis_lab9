//
// Created by hatomist on 9/15/19.
//

#ifndef LIS_LAB3_CHECK_H
#define LIS_LAB3_CHECK_H

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

double
get_double (char *prompt, bool non_negative, bool cant_be_zero);

long long
get_ll (char *prompt);

unsigned long long
get_ull (char *prompt);


#endif //LIS_LAB3_CHECK_H
