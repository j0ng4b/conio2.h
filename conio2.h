#include <stdlib.h>
#include <ncurses.h>


#define _NOCURSOR 0
#define _SOLIDCURSOR 2
#define _NORMALCURSOR 1

#undef getch
#define kbhit() conio2_kbhit()
#define getch() conio2_getch(0)
#define getche() conio2_getch(1)

#define textbackground(color) conio2_change_text_colors(color, 0)
#define textcolor(color) conio2_change_text_colors(color, 1)

#define cprintf(...) (conio2_init_ncurses(), printw(__VA_ARGS__), refresh())
#define cputs(string) (conio2_init_ncurses(), printw("%s", string), refresh())
#define cscanf(...) (conio2_init_ncurses(), scanw(__VA_ARGS__))

#define clrscr() clear()
#define clreol() clrtoeol()

#define gotoxy(x, y) move((y) - 1, (x) - 1)

#define _setcursortype(type) curs_set(type)


enum COLORS {
    BLACK   =   COLOR_BLACK,
    RED     =     COLOR_RED,
    GREEN   =   COLOR_GREEN,
    YELLOW  =  COLOR_YELLOW,
    BLUE    =    COLOR_BLUE,
    MAGENTA = COLOR_MAGENTA,
    CYAN    =    COLOR_CYAN,
    WHITE   =   COLOR_WHITE,
};


static int initialized = 0;
static int foreground_color;
static int background_color;
static int kbhit_key = ERR;


static void conio2_quit_ncurses(void)
{
    endwin();
}

static void conio2_init_colors(void)
{
    int i, j;

    for (i = 0; i < 8; ++i)
        for (j = 0; j < 8; ++j)
            init_pair((i * 8) + j, j, i);
}

static void conio2_init_ncurses(void)
{
    if (initialized)
        return;

    initscr();
    nonl();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    if (has_colors() == FALSE)
        fprintf(stderr, "This terminal can't display colors!");
    else
        conio2_init_colors();

    foreground_color = WHITE;
    background_color = BLACK;

    atexit(conio2_quit_ncurses);
    initialized = 1;
}

static void conio2_change_text_colors(int color, int is_foreground)
{
    if (!initialized)
        conio2_init_ncurses();

    if (is_foreground)
        foreground_color = color;
    else
        background_color = color;

    attron(COLOR_PAIR((background_color * 8) + foreground_color));
}

static int conio2_kbhit(void)
{
    if (!initialized)
        conio2_init_ncurses();

    timeout(0);
    kbhit_key = wgetch(stdscr);
    timeout(-1);

    return kbhit_key != ERR;
}

static int conio2_getch(int should_echo)
{
    if (!initialized)
        conio2_init_ncurses();

    int key = kbhit_key;
    kbhit_key = ERR;

    if (key != ERR) {
        if (should_echo)
            echochar(key);

        return key;
    }

    key = wgetch(stdscr);
    if (should_echo)
        echochar(key);

    return key;
}

/*
int main()
{
    textbackground(RED);
    cputs("Ola");
    getch();
    return 0;
}
*/

