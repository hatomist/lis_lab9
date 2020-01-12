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

int main(int argc, char *argv[]);
void help(char *exec_name);
int tui(FILE *pFile);
int show_records(FILE *pFile);
int add_records(FILE *pFile);
int delete_records(FILE *pFile);
int sort_records(FILE *pFile);
int delete_file(FILE *pFile);
int exit_program(FILE *pFile);

#endif //LIS_LAB9_MAIN_H
