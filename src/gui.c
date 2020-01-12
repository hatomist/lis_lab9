#include "gui.h"


int gui(FILE *pFile)
{

    WINDOW *w[3];
    PANEL  *p[3];
    PANEL  *top;
    int ch;

    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    init_view_w(w + VIEWER);
    init_help_w(w + HELP);
    init_editor_w(w + EDITOR);


    /* Attach a panel to each window */ 	/* Order is bottom up */
    for (size_t i = 0; i < 3; i++)
        p[i] = new_panel(w[i]);

    /* Set up the user pointers to the next panel */
    for (size_t i = 0; i < 3; i++)
        set_panel_userptr(p[i%3], p[(i+1)%3]);

    /* Update the stacking order. 2nd panel will be on top */
    update_panels();

    /* Show it on the screen */
    attron(COLOR_PAIR(4));
    mvprintw(LINES - 1, 0, "Some menu text here\n");
    attroff(COLOR_PAIR(4));
    doupdate();

    top = p[2];
    while((ch = getch()) != KEY_F(1))
    {	switch(ch)
        {	case 9:
                top = (PANEL *)panel_userptr(top);
                top_panel(top);
                break;
        }
        update_panels();
        doupdate();
    }
    endwin();
    return 0;


}
/* Put all the windows */
//void init_wins(WINDOW **wins, int n)
//{
//    int x, y, i;
//    char label[80];
//
//    y = 2;
//    x = 10;
//    for(i = 1; i < n-1; ++i)
//    {
//        wins[i] = newwin(NLINES, NCOLS, y, x);
//        sprintf(label, "Window Number %d", i + 1);
//        win_show(wins[i], label, i + 1);
//        y += 3;
//        x += 7;
//    }
//}

void init_view_w(WINDOW **view)
{
    char label[80];

    *view = newwin(LINES-1, COLS, 0, 0);
    sprintf(label, "Semi-editor");
    win_show(*view, label, 3);
}

void init_help_w(WINDOW **view)
{
    char label[80];

    *view = newwin(19, COLS-4, 3, 2);
    sprintf(label, "Help");
    win_show(*view, label, 3);
}

void init_editor_w(WINDOW **view)
{
    char label[80];

    *view = newwin(7, 40, 8, 20);
    sprintf(label, "Editor");
    win_show(*view, label, 3);
}


/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color)
{
    int startx, starty, height, width;

    getbegyx(win, starty, startx);
    getmaxyx(win, height, width);

    box(win, 0, 0);
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    mvwaddch(win, 2, width - 1, ACS_RTEE);

    print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if(win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if(startx != 0)
        x = startx;
    if(starty != 0)
        y = starty;
    if(width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}
