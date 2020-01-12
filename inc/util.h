#ifndef LIS_LAB9_UTIL_H
#define LIS_LAB9_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

int multiselect_question(char *array, int count);
static int multiselect_parse(char *array, int count, char *response);
size_t strtrim(char *str);
static int parseindex(char *s, int *val, int min, int max);

#define MENU_ITEM_COUNT 6

#endif //LIS_LAB9_UTIL_H
