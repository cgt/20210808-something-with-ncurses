#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char **argv) {
	initscr();
	cbreak();
	noecho();
	curs_set(false);
	nodelay(stdscr, true);
	keypad(stdscr, true);

	int max_y = 0;
	int max_x = 0;
	getmaxyx(stdscr, max_y, max_x);

	int x = 0;
	int y = 0;
	bool zoom = false;
	while (1) {
		clear();
		mvprintw(y, x, "o");
		refresh();
		usleep(1000);
		int ch = getch();
		int steps = zoom ? 3 : 1;
		switch (ch) {
			case KEY_LEFT:
				x -= steps;
				break;
			case KEY_RIGHT:
				x += steps;
				break;
			case KEY_UP:
				y -= steps;
				break;
			case KEY_DOWN:
				y += steps;
				break;
			case 'z':
				zoom = !zoom;
				break;
		}
	}

	endwin();
}
