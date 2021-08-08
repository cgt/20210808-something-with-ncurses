#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <iostream>

static bool quit = false;

static void handleInterrupt(int _) {
	quit = true;
}

struct Position {
	int y;
	int x;
};

bool operator==(const Position& lhs, const Position& rhs) {
	return lhs.y == rhs.y && lhs.x == rhs.x;
}

bool operator!=(const Position& lhs, const Position& rhs) {
	return !(lhs == rhs);
}

class Game {
	int max_y = 0;
	int max_x = 0;
	Position current{1, 1};
	std::vector<Position> path;

	void setup();
	void goLeft();

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

void Game::goLeft() {
	auto x = current.x-1;
	if (x > max_x) {
		x = max_x;
	} else if (x < 1) {
		x = 1;
	}
	current.x = x;
}


void Game::run() {
	setup();
	getmaxyx(stdscr, max_y, max_x);
	max_y -= 2;
	max_x -= 2;

	bool leap = false;
	while (1) {
		if (quit) {
			break;
		}
		int ch = getch();
		int steps = leap ? 3 : 1;

		auto old = current;

		switch (ch) {
			case KEY_LEFT:
				goLeft();
				break;
			case KEY_RIGHT:
				current.x += steps;
				break;
			case KEY_UP:
				current.y -= steps;
				break;
			case KEY_DOWN:
				current.y += steps;
				break;
			case 'z':
				leap = !leap;
				break;
		}
		if (current.y > max_y) {
			current.y = max_y;
		}
		if (current.y < 1) {
			current.y = 1;
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
		mvprintw(current.y, current.x, "o");
		if (old != current) {
			path.push_back(current);
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
