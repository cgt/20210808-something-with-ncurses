#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>

static bool quit = false;

static void handleInterrupt(int _) {
	quit = true;
}

struct Position {
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
	std::vector<Position> trail2;
	while (1) {
		if (quit) {
			break;
		}
		int ch = getch();
		int steps = zoom ? 3 : 1;

		if (ch != ERR) {
			Position p{y, x};
			trail2.push_back(p);
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
		for (auto p : trail2) {
			mvaddch(p.y, p.x, ACS_DIAMOND);
		}
		mvprintw(y, x, "o");
		refresh();
		usleep(5000);
	}

	endwin();
}
