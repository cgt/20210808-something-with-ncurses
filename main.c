#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

static bool quit = false;

static void handleInterrupt(int _) {
	quit = true;
}

typedef struct {
	int y;
	int x;
} Position;

int main(int argc, char **argv) {
	signal(SIGINT, handleInterrupt);

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
	int mark_x = 0;
	int mark_y = 0;
	Position *trail[] = {{0, 0}, {0, 0}};
	while (1) {
		if (quit) {
			break;
		}
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
			case 'a':
				mark_x = x;
				mark_y = y;
		}
		clear();
		mvaddch(mark_y, mark_x, 'x');
		mvprintw(y, x, "o");
		refresh();
		usleep(1000);
	}

	endwin();
}
