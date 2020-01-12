#include "stdio.h"
#include <panel.h>
#include <string.h>

#ifndef LIS_LAB9_GUI_H
#define LIS_LAB9_GUI_H

int gui(FILE *pFile);
void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void init_view_w(WINDOW **view);
void init_help_w(WINDOW **view);
void init_editor_w(WINDOW **view);



#define NLINES 10
#define NCOLS 40

#define VIEWER 0
#define EDITOR 1
#define HELP 2

#endif //LIS_LAB9_GUI_H
