#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>

char **get_big_char(char ch)
{
    char **result = malloc(sizeof(char *) * 7); // 7 elements in the array

    switch (tolower(ch))
    {
    case 'a':
        result[0] = strdup(" ##### ");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#######");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#     #");
        break;
    case 'b':
        result[0] = strdup("###### ");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("###### ");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("###### ");
        break;
    case 'c':
        result[0] = strdup(" ##### ");
        result[1] = strdup("#     #");
        result[2] = strdup("#      ");
        result[3] = strdup("#      ");
        result[4] = strdup("#      ");
        result[5] = strdup("#     #");
        result[6] = strdup(" ##### ");
        break;
    case 'd':
        result[0] = strdup("#####  ");
        result[1] = strdup("#    # ");
        result[2] = strdup("#     #");
        result[3] = strdup("#     #");
        result[4] = strdup("#     #");
        result[5] = strdup("#    # ");
        result[6] = strdup("#####  ");
        break;
    case 'e':
        result[0] = strdup("#######");
        result[1] = strdup("#      ");
        result[2] = strdup("#      ");
        result[3] = strdup("#######");
        result[4] = strdup("#      ");
        result[5] = strdup("#      ");
        result[6] = strdup("#######");
        break;
    case 'f':
        result[0] = strdup("#######");
        result[1] = strdup("#      ");
        result[2] = strdup("#      ");
        result[3] = strdup("#######");
        result[4] = strdup("#      ");
        result[5] = strdup("#      ");
        result[6] = strdup("#      ");
        break;
    case 'g':
        result[0] = strdup("#######");
        result[1] = strdup("#      ");
        result[2] = strdup("#      ");
        result[3] = strdup("#  ####");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#######");
        break;
    case 'h':
        result[0] = strdup("#     #");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#######");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#     #");
        break;
    case 'i':
        result[0] = strdup("#######");
        result[1] = strdup("   #   ");
        result[2] = strdup("   #   ");
        result[3] = strdup("   #   ");
        result[4] = strdup("   #   ");
        result[5] = strdup("   #   ");
        result[6] = strdup("#######");
        break;
    case 'j':
        result[0] = strdup("#######");
        result[1] = strdup("     # ");
        result[2] = strdup("     # ");
        result[3] = strdup("     # ");
        result[4] = strdup("#    # ");
        result[5] = strdup("#    # ");
        result[6] = strdup(" ####  ");
        break;
    case 'k':
        result[0] = strdup("#   #  ");
        result[1] = strdup("#  #   ");
        result[2] = strdup("# #    ");
        result[3] = strdup("##     ");
        result[4] = strdup("# #    ");
        result[5] = strdup("#  #   ");
        result[6] = strdup("#   #  ");
        break;
    case 'l':
        result[0] = strdup("#      ");
        result[1] = strdup("#      ");
        result[2] = strdup("#      ");
        result[3] = strdup("#      ");
        result[4] = strdup("#      ");
        result[5] = strdup("#      ");
        result[6] = strdup("#######");
        break;
    case 'm':
        result[0] = strdup("#     #");
        result[1] = strdup("##   ##");
        result[2] = strdup("# # # #");
        result[3] = strdup("#  #  #");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#     #");
        break;
    case 'n':
        result[0] = strdup("#     #");
        result[1] = strdup("##    #");
        result[2] = strdup("# #   #");
        result[3] = strdup("#  #  #");
        result[4] = strdup("#   # #");
        result[5] = strdup("#    ##");
        result[6] = strdup("#     #");
        break;
    case 'o':
        result[0] = strdup("#######");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#     #");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#######");
        break;
    case 'p':
        result[0] = strdup("###### ");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("###### ");
        result[4] = strdup("#      ");
        result[5] = strdup("#      ");
        result[6] = strdup("#      ");
        break;
    case 'q':
        result[0] = strdup("#######");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#     #");
        result[4] = strdup("#######");
        result[5] = strdup("     # ");
        result[6] = strdup("      #");
        break;
    case 'r':
        result[0] = strdup("###### ");
        result[1] = strdup("#     #");
        result[2] = strdup("#    # ");
        result[3] = strdup("#####  ");
        result[4] = strdup("#    # ");
        result[5] = strdup("#     #");
        result[6] = strdup("#     #");
        break;
    case 's':
        result[0] = strdup("#######");
        result[1] = strdup("#      ");
        result[2] = strdup("#      ");
        result[3] = strdup("#######");
        result[4] = strdup("      #");
        result[5] = strdup("      #");
        result[6] = strdup("#######");
        break;
    case 't':
        result[0] = strdup("#######");
        result[1] = strdup("   #   ");
        result[2] = strdup("   #   ");
        result[3] = strdup("   #   ");
        result[4] = strdup("   #   ");
        result[5] = strdup("   #   ");
        result[6] = strdup("   #   ");
        break;
    case 'u':
        result[0] = strdup("#     #");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#     #");
        result[4] = strdup("#     #");
        result[5] = strdup("#     #");
        result[6] = strdup("#######");
        break;
    case 'v':
        result[0] = strdup("#     #");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#     #");
        result[4] = strdup(" #   # ");
        result[5] = strdup("  # #  ");
        result[6] = strdup("   #   ");
        break;
    case 'w':
        result[0] = strdup("#     #");
        result[1] = strdup("#     #");
        result[2] = strdup("#     #");
        result[3] = strdup("#  #  #");
        result[4] = strdup("# # # #");
        result[5] = strdup("##   ##");
        result[6] = strdup("#     #");
        break;
    case 'x':
        result[0] = strdup("#     #");
        result[1] = strdup(" #   # ");
        result[2] = strdup("  # #  ");
        result[3] = strdup("   #   ");
        result[4] = strdup("  # #  ");
        result[5] = strdup(" #   # ");
        result[6] = strdup("#     #");
        break;
    case 'y':
        result[0] = strdup("#     #");
        result[1] = strdup(" #   # ");
        result[2] = strdup("  # #  ");
        result[3] = strdup("   #   ");
        result[4] = strdup("   #   ");
        result[5] = strdup("   #   ");
        result[6] = strdup("   #   ");
        break;
    case 'z':
        result[0] = strdup("#######");
        result[1] = strdup("     # ");
        result[2] = strdup("    #  ");
        result[3] = strdup("   #   ");
        result[4] = strdup("  #    ");
        result[5] = strdup(" #     ");
        result[6] = strdup("#######");
        break;
    case ' ':
        result[0] = strdup("       ");
        result[1] = strdup("       ");
        result[2] = strdup("       ");
        result[3] = strdup("       ");
        result[4] = strdup("       ");
        result[5] = strdup("       ");
        result[6] = strdup("       ");
    }
    return result;
}

void free_big_char(char **ch)
{
    int i = 0;
    for (i = 0; i < 7; i++)
    {
        free(ch[i]);
    }
    free(ch);
}

void print_string_big_chars(WINDOW *win, char *str, int starty, int startx)
{
    int i = 0, j = 0;
    int initial_y = starty; //, initial_x = startx;
    char **ch = NULL;

    for (i = 0; i < strlen(str); i++)
    {
        ch = get_big_char(str[i]);
        for (j = 0; j < 7; j++)
        {
            mvwprintw(win, starty, startx, ch[j]);
            starty += 1;
        }
        refresh();
        starty = initial_y;
        startx += 8;
        free_big_char(ch);
    }
}