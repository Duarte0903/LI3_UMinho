#include <ncurses.h>
#include <string.h>
#include "../includes/interactive.h"
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/chars.h"

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(local_win);
    return local_win;
}

void print_in_window(WINDOW *win, char *str, int standout)
{
    if (standout == 1)
    {
        wattron(win, A_STANDOUT);
    }
    int lines, columns;
    getmaxyx(win, lines, columns);
    mvwprintw(win, lines / 2, columns / 2 - strlen(str) / 2, str);
    wrefresh(win);
    wattroff(win, A_STANDOUT);
}

// void delete_window(WINDOW *win)
// {
//     wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
//     wrefresh(win);
//     delwin(win);
// }

char *choose_dataset()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);

    int choice;
    int highlight = 0;

    int starty_title = 1;
    int startx_title = COLS / 2 - (strlen("Choose a dataset") * 8) / 2; // 7 is the width of each char + 1(space added after each char printed)

    print_string_big_chars("Choose a dataset", starty_title, startx_title);

    WINDOW *c1 = create_newwin(5, 50, LINES / 2 - 10, COLS / 2 - 25);
    WINDOW *c2 = create_newwin(5, 50, LINES / 2 - 5, COLS / 2 - 25);
    WINDOW *c3 = create_newwin(5, 50, LINES / 2, COLS / 2 - 25);
    WINDOW *c4 = create_newwin(5, 50, LINES / 2 + 5, COLS / 2 - 25);
    WINDOW *c5 = create_newwin(5, 50, LINES / 2 + 10, COLS / 2 - 25);

    char *data_path = NULL;

    int cycle_flag = 1;
    while (cycle_flag)
    {

        print_in_window(c1, "Regular dataset, without invalid entries.", 0);
        print_in_window(c2, "Regular dataset, with invalid entries.", 0);
        print_in_window(c3, "Large dataset, without invalid entries.", 0);
        print_in_window(c4, "Large dataset, with invalid entries.", 0);
        print_in_window(c5, "Quit", 0);

        refresh();

        if (highlight == 0)
        {
            print_in_window(c1, "Regular dataset, without invalid entries.", 1);
            refresh();
        };
        if (highlight == 1)
        {
            print_in_window(c2, "Regular dataset, with invalid entries.", 1);
            refresh();
        };
        if (highlight == 2)
        {
            print_in_window(c3, "Large dataset, without invalid entries.", 1);
            refresh();
        };
        if (highlight == 3)
        {
            print_in_window(c4, "Large dataset, with invalid entries.", 1);
            refresh();
        };
        if (highlight == 4)
        {
            print_in_window(c5, "Quit", 1);
            refresh();
        };

        choice = getch(); // get input

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight < 0)
            {
                highlight = 0;
            }
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight >= 5)
            {
                highlight = 4;
            }
            break;
        case 10: // Enter
            if (highlight == 0)
            {
                cycle_flag = 0;
                data_path = malloc(70 * sizeof(char));
                strcpy(data_path, "../trabalho-pratico/datasets/small-valid-dataset");
            }
            if (highlight == 1)
            {
                cycle_flag = 0;
                data_path = malloc(70 * sizeof(char));
                strcpy(data_path, "../trabalho-pratico/datasets/small-invalid-dataset");
            }
            if (highlight == 2)
            {
                cycle_flag = 0;
                data_path = malloc(70 * sizeof(char));
                strcpy(data_path, "../trabalho-pratico/datasets/large-valid-dataset");
            }
            if (highlight == 3)
            {
                cycle_flag = 0;
                data_path = malloc(70 * sizeof(char));
                strcpy(data_path, "../trabalho-pratico/datasets/large-invalid-dataset");
            }
            if (highlight == 4)
            {
                cycle_flag = 0;
            }
            break;
        }
    }

    clear();
    endwin();

    return data_path;
}

void print_waiting_on_catalogs() // basically a wrapper to start ncurses mode on main.c and print, might change
{
    initscr();
    noecho();

    print_string_big_chars("Building Catalogs", LINES / 2 - 4, COLS / 2 - (strlen("Building Catalogs") * 8) / 2);
}