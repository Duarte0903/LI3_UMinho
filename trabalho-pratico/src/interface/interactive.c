#include <curses.h>
#include <locale.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "../includes/interface/interactive.h"
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/interface/chars.h"

#define MAX_QUERY_ARGS 4

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(local_win);
    return local_win;
}

void print_in_window_centered(WINDOW *win, char *str, int standout)
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

void delete_window(WINDOW *win)
{
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void clear_window_contents(WINDOW *win)
{
    int rows, cols;
    getmaxyx(win, rows, cols);

    for (int row = 1; row < rows - 1; row++)
    {
        for (int col = 1; col < cols - 1; col++)
        {
            mvwaddch(win, row, col, ' ');
        }
    }
    wrefresh(win);
}

void print_page(WINDOW *win, int starty, int startx, int output_lines, char *str)
{
    char *ptr = str;
    char *line = malloc(100 * sizeof(char));

    while (starty < 0)
    {
        if (ptr[0] == '\n')
            starty++;
        ptr++;
    }

    while (sscanf(ptr, "%[^\n]\n", line) == 1 && starty <= output_lines)
    {
        mvwaddstr(win, starty, startx, line);
        wrefresh(win);
        starty++;
        ptr += strlen(line) + 1;
    }
    free(line);
}

char *choose_dataset() // starts ncurses on main
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int choice;
    int highlight = 0;

    int starty_title = 1;
    int startx_title = COLS / 2 - (strlen("Choose a dataset") * 8) / 2; // 7 is the width of each char + 1(space added after each char printed)

    print_string_big_chars(stdscr, "Choose a dataset", starty_title, startx_title);

    WINDOW *c1 = create_newwin(LINES / 8, COLS / 4, LINES / 2 - 2 * LINES / 8, COLS / 2 - COLS / 8); // used the macros in order to allow some limited resizing
    WINDOW *c2 = create_newwin(LINES / 8, COLS / 4, LINES / 2 - LINES / 8, COLS / 2 - COLS / 8);
    WINDOW *c3 = create_newwin(LINES / 8, COLS / 4, LINES / 2, COLS / 2 - COLS / 8);
    WINDOW *c4 = create_newwin(LINES / 8, COLS / 4, LINES / 2 + LINES / 8, COLS / 2 - COLS / 8);
    WINDOW *c5 = create_newwin(LINES / 8, COLS / 4, LINES / 2 + 2 * LINES / 8, COLS / 2 - COLS / 8);

    char *data_path = NULL;

    int cycle_flag = 1;
    while (cycle_flag)
    {

        print_in_window_centered(c1, "Regular dataset, without invalid entries.", 0);
        print_in_window_centered(c2, "Regular dataset, with invalid entries.", 0);
        print_in_window_centered(c3, "Large dataset, without invalid entries.", 0);
        print_in_window_centered(c4, "Large dataset, with invalid entries.", 0);
        print_in_window_centered(c5, "Quit", 0);

        refresh();

        if (highlight == 0)
        {
            print_in_window_centered(c1, "Regular dataset, without invalid entries.", 1);
            refresh();
        };
        if (highlight == 1)
        {
            print_in_window_centered(c2, "Regular dataset, with invalid entries.", 1);
            refresh();
        };
        if (highlight == 2)
        {
            print_in_window_centered(c3, "Large dataset, without invalid entries.", 1);
            refresh();
        };
        if (highlight == 3)
        {
            print_in_window_centered(c4, "Large dataset, with invalid entries.", 1);
            refresh();
        };
        if (highlight == 4)
        {
            print_in_window_centered(c5, "Quit", 1);
            refresh();
        };

        choice = getch();

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

    return data_path;
}

void print_waiting_on_catalogs()
{
    print_string_big_chars(stdscr, "Building Catalogs", LINES / 2 - 4, COLS / 2 - (strlen("Building Catalogs") * 8) / 2); // 4 because the length of a big char is 7 and 4 is close to half of that, leaving it somewhat centered
}

void display_query_results(WINDOW *border_win, char *input, ...)
{
    noecho();
    va_list args;
    va_start(args, input);

    int border_lines, border_cols;
    getmaxyx(border_win, border_lines, border_cols);

    WINDOW *win = newwin(border_lines - 2, border_cols - 2, LINES / 4 + 1, COLS / 4 + 1); // border starting position + 1

    int output_lines = getmaxy(win);

    Output_Type output_struct = parse_query_interactive(input, MAX_QUERY_ARGS, args);

    setlocale(LC_ALL, "");

    int lines = 1;
    for (int i = 0; i < (int)strlen(output_struct->Type.str); i++)
    {
        if (output_struct->Type.str[i] == '\n')
            lines++;
    }
    int n_pages = ceil((double)lines / output_lines);

    print_page(win, 0, 0, output_lines, output_struct->Type.str);
    mvprintw(LINES / 4 - 1, COLS / 4, "1 of %d", n_pages);
    mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");

    int y_setter = 0; // this multiplies by (-output_lines) in order to have the starting y of the string placed in a way that the window and the correct chunk of the string match each other
    int cycle_flag = 1;
    while (cycle_flag)
    {
        int choice = getch();
        switch (choice)
        {
        case KEY_RIGHT:
            y_setter++;
            if (y_setter >= n_pages - 1)
            {
                y_setter = n_pages - 1;
            }
            wclear(win);
            print_page(win, (-output_lines) * y_setter, 0, output_lines, output_struct->Type.str);

            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            mvprintw(LINES / 4 - 1, COLS / 4, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");
            refresh();
            break;
        case KEY_LEFT:
            y_setter--;
            if (y_setter <= 0)
            {
                y_setter = 0;
            }
            wclear(win);
            print_page(win, (-output_lines) * y_setter, 0, output_lines, output_struct->Type.str);

            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            mvprintw(LINES / 4 - 1, COLS / 4, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");
            refresh();
            break;
        case 10:
            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            wclear(win);
            wrefresh(win);
            refresh();
            cycle_flag = 0;
            break;
        }
    }
    va_end(args);
    free(output_struct->Type.str);
    free(output_struct);
}

void launch_queries_interface(Users_Catalog users_catalog, Drivers_Catalog drivers_catalog, Rides_Catalog rides_catalog)
{
    clear();
    print_string_big_chars(stdscr, "Queries", 1, COLS / 2 - (strlen("Queries") * 8) / 2);

    mvprintw(9, COLS / 2 - strlen("Type 'exit' to quit.") / 2, "Type 'exit' to quit.");
    move(LINES - LINES / 16, COLS / 4);
    refresh();

    WINDOW *input_window = newwin(LINES / 16, COLS / 2, LINES - LINES / 16, COLS / 4);
    WINDOW *output_window = create_newwin(LINES / 1.5, COLS / 2, LINES / 4, COLS / 4); // dividing by 1.5 and 2 so that it's a rectangle which takes up most of the window and allows for some limited freedom in resolution, dividing by four so that it's centered (starty and startx are the top left corner)

    int query_number;
    char remaining_input;
    char *str = malloc(100 * sizeof(char));
    do
    {
        echo();
        curs_set(1);
        wgetstr(input_window, str);
        curs_set(0);

        if (sscanf(str, "%d %c", &query_number, &remaining_input) == 2)
        {
            display_query_results(output_window, str, users_catalog, drivers_catalog, rides_catalog);
        }

        wclear(input_window);
    } while (strcmp(str, "exit"));

    delwin(input_window);
    delete_window(output_window);
    clear();
    free(str);
}

void launch_main_interface(int n_args, ...)
{
    clear();
    keypad(stdscr, true);

    va_list args;
    va_start(args, n_args);

    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);

    int highlight = 0;
    int cycle_flag = 1;

    while (cycle_flag)
    {

        print_string_big_chars(stdscr, "Uber Database", 5, COLS / 2 - (strlen("Uber Database") * 8) / 2);
        print_string_big_chars(stdscr, "LI3", LINES - 5 * 2, COLS / 2 - (strlen("LI3") * 8) / 2);

        WINDOW *queries_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 - LINES / 8, COLS / 2 - COLS / 8); // using the macros instead of simple numbers in order to allow some limited resizing
        WINDOW *quit_window = create_newwin(LINES / 8, COLS / 4, LINES / 2, COLS / 2 - COLS / 8);

        print_in_window_centered(queries_option, "Execute Queries", 0);
        print_in_window_centered(quit_window, "Quit", 0);

        refresh();

        if (highlight == 0)
        {
            print_in_window_centered(queries_option, "Execute Queries", 1);
            refresh();
        }
        else if (highlight == 1)
        {
            print_in_window_centered(quit_window, "Quit", 1);
            refresh();
        }

        int choice = getch();

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight <= 0)
            {
                highlight = 0;
            };
            delete_window(queries_option);
            delete_window(quit_window);
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight >= 1)
            {
                highlight = 1;
            };
            delete_window(queries_option);
            delete_window(quit_window);
            break;
        case 10:
            if (highlight == 0)
            {
                launch_queries_interface(users_catalog, drivers_catalog, rides_catalog);
            }
            else if (highlight == 1)
            {
                cycle_flag = 0;
            }
            break;
        }
    }

    va_end(args);
    endwin();
}