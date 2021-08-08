#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

static bool quit = false;

static void handleInterrupt(int _) {
	quit = true;
}

struct Position{
	int y;
	int x;
};

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
	Position trail[10] = {{0, 0}};
	int next_trail = 0;
	while (1) {
		if (quit) {
			break;
		}
		int ch = getch();
		int steps = zoom ? 3 : 1;

		if (ch != ERR) {
			trail[next_trail].y = y;
			trail[next_trail].x = x;
			next_trail++;
			if (next_trail > sizeof(trail)/sizeof(trail[0])) {
				next_trail = 0;
			}
		}

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
		erase();
		border(0, 0, 0, 0, 0, 0, 0, 0);
		for (int i = 0; i < sizeof(trail)/sizeof(trail[0]); i++) {
			Position p = trail[i];
			mvaddch(p.y, p.x, ACS_DIAMOND);
		}
		mvprintw(y, x, "o");
		refresh();
		usleep(5000);
	}

	endwin();
}