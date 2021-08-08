#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <vector>
#include <utility>
#include <iostream>

static bool quit = false;

static void handleInterrupt(int _) {
	quit = true;
}

struct Position {
	int y;
	int x;
};

class Game {
	int x = 1;
	int y = 1;
	std::vector<Position> path;

	void setup();

	public:
	void run();
};

void Game::setup() {
	initscr();
	cbreak();
	noecho();
	curs_set(false);
	nodelay(stdscr, true);
	keypad(stdscr, true);
}


void Game::run() {
	setup();
	int max_y = 0;
	int max_x = 0;
	getmaxyx(stdscr, max_y, max_x);

	bool zoom = false;
	while (1) {
		if (quit) {
			break;
		}
		int ch = getch();
		int steps = zoom ? 3 : 1;

		auto oldx = x;
		auto oldy = y;

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

		for (int yy = 10; yy < 20; yy++) {
			for (int xx = 10; xx < 20; xx++) {
				auto trampled = false;
				for (auto position : path) {
					if (position.y == yy && position.x == xx) {
						trampled = true;
					}
				}
				if (!trampled) {
					mvaddch(yy, xx, ACS_BLOCK);
				}
			}
		}
		mvprintw(y, x, "o");
		if (oldx != x || oldy != y) {
			Position current_position{y, x};
			path.push_back(current_position);
		}
		refresh();
		usleep(1000);
	}

	endwin();

	for (auto position : path) {
		std::cout << "x=" << position.x << " y=" << position.y << '\n';
	}
}

int main(int argc, char **argv) {
	signal(SIGINT, handleInterrupt);

	Game game;
	game.run();
}
