#define _XOPEN_SOURCE_EXTENDED
#include <locale.h>
#include <wchar.h>
#include <ncursesw/curses.h>

int main() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    curs_set(FALSE);

    mvaddwstr(2, 5, L"🌀 coilboi");
    mvaddwstr(4, 5, L"press any key to exit");

    refresh();
    getch();
    endwin();

    return 0;
}
