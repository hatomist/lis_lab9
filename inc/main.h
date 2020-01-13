#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include "check.h"
#include "util.h"

#ifndef LIS_LAB9_MAIN_H
#define LIS_LAB9_MAIN_H

#define SORT_OFF   0
#define SORT_OBL   1
#define SORT_AREA  2
#define SORT_POPUL 3

typedef struct {char *title; double area; double population;} record;

int main(int argc, char *argv[]);
void help(char *exec_name);
int tui(FILE *pFile);
int show_records(const record **records, const size_t *records_num);
int add_records(record **records, size_t *records_num);
int delete_records(record **records, size_t *records_num);
int change_records_sort(record **records, size_t *records_num);
int delete_file(record **records, size_t *records_num);
int sort_records(record **records, size_t *records_num);
int exit_program(const record **records, const size_t *records_num);
size_t get_file_size(FILE *pFile);



#endif //LIS_LAB9_MAIN_H
