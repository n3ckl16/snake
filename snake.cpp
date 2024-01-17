#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>


using namespace std;

class Point {
public:
	int x, y;
	Point(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}
	Point copy() {
		return Point(this->x, this->y);
	}

};

class Part : public Point {
public:
	Point oldPos;
	Part(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}
	void move(int x, int y) {
		if (this->x == x && this->y == y) return;
		this->oldPos = this->copy();
		this->x = x;
		this->y = y;
	}
	void move(Point p) {
		if (p.x == this->x && this->y == p.y) return;
		this->oldPos = this->copy();
		this->x = p.x;
		this->y = p.y;
	}
};

class Snake {
public:
	static const unsigned short maxLen = 100;
	char headSym = 'v';
	char bodySym = 15;
	Part body[maxLen] = {};
	Part direction = Part(0, 0);
	int points = 0;
	int slowness = 20;
	int wait = 200;
	int maxWait = 200;
	char curLetter = 'q';

	Snake(int x = 0, int y = 0) {
		this->body[0] = Part(x, y);
		this->points = 0;
	}
	void input() {

		if (_kbhit()) {
			this->curLetter = _getch();
		}

		switch (this->curLetter) {
		case 'w':
			this->headSym = 'v';
			this->direction.y = -1;
			this->direction.x = 0;
			break;
		case 's':
			this->headSym = '^';
			this->direction.y = 1;
			this->direction.x = 0;
			break;
		case 'a':
			this->headSym = '>';
			this->direction.y = 0;
			this->direction.x = -1;
			break;
		case 'd':
			this->headSym = '<';
			this->direction.y = 0;
			this->direction.x = 1;
			break;

		case 'W':
			this->headSym = 'v';
			this->direction.y = -1;
			this->direction.x = 0;
			break;
		case 'S':
			this->headSym = '^';
			this->direction.y = 1;
			this->direction.x = 0;
			break;
		case 'A':
			this->headSym = '>';
			this->direction.y = 0;
			this->direction.x = -1;
			break;
		case 'D':
			this->headSym = '<';
			this->direction.y = 0;
			this->direction.x = 1;
			break;

		}

	}

};

class Game {
public:
	COORD c;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };

	string statistic;
	string name;
	Snake snake;
	Part apple;
	const static unsigned short width = 40, height = 20;
	string map[height][width] = {};
	bool isOver = false;
	char space = ' ';
	char appleSym = 'O';
	Part prev1;
	Part prev2;



	Game() {
		SetConsoleCursorInfo(h, &cci);
		this->snake = Snake(this->width / 2, this->height / 2);
		this->apple = Part(this->width / 2, this->height / 2 - 4);

		srand((unsigned)time(0));
	}

	void loop() {
		this->draw();

		while (!this->isOver) {

			this->c.X = 0;
			this->c.Y = 0;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 8);
			cout << "\tPoints: " << this->snake.points << "\tSpeed: " << (((this->snake.maxWait - this->snake.wait) == 0) ? 1 : (this->snake.maxWait - this->snake.wait));

			if (this->apple.x == this->snake.body[0].x && this->apple.y == this->snake.body[0].y) {
				this->snake.points++;

				this->c.X = this->apple.x;
				this->c.Y = this->apple.y;
				SetConsoleCursorPosition(h, c);
				putchar(this->space);

				this->apple.x = (rand() % (this->width - 2) + 2);
				this->apple.y = (rand() % (this->height - 2) + 2);

				for (int i = 1; i <= this->snake.points; i++) {
					if (this->apple.x == this->snake.body[i].x && this->apple.y == this->snake.body[i].y) {
						this->apple.x = (rand() % (this->width - 2) + 2);
						this->apple.y = (rand() % (this->height - 2) + 2);
					}
					else {
						this->c.X = this->apple.x;
						this->c.Y = this->apple.y;

					}
				}
				SetConsoleCursorPosition(h, c);
				SetConsoleTextAttribute(h, 12);
				putchar(this->appleSym);

				if (this->snake.points % 4 == 0 && this->snake.wait > 60) this->snake.wait -= this->snake.slowness;
			}


			Sleep(this->snake.wait);
			this->snake.input();
			for (unsigned short i = 0; i <= this->snake.points; i++) {
				this->c.Y = this->snake.body[0].y;
				this->c.X = this->snake.body[0].x;
				SetConsoleCursorPosition(h, c);
				putchar(this->space);

				if (i == 0) this->snake.body[i].move(this->snake.body[i].x + this->snake.direction.x, this->snake.body[i].y + this->snake.direction.y);
				else {
					this->c.Y = this->snake.body[i].y;
					this->c.X = this->snake.body[i].x;
					SetConsoleCursorPosition(h, c);
					putchar(this->space);

					this->snake.body[i].move(this->snake.body[i - 1].oldPos.x, this->snake.body[i - 1].oldPos.y);


					this->c.Y = this->snake.body[i].y;
					this->c.X = this->snake.body[i].x;
					SetConsoleCursorPosition(h, c);
					SetConsoleTextAttribute(h, 10);
					putchar(this->snake.bodySym);
				}
			}
			if (this->snake.body[0].x >= this->width + 1 || this->snake.body[0].x <= 0 || this->snake.body[0].y == this->height + 2 || this->snake.body[0].y <= 1) break;

			for (unsigned short i = 1; i < this->snake.points; i++) if (this->snake.body[0].x == this->snake.body[i].oldPos.x && this->snake.body[0].y == this->snake.body[i].oldPos.y) this->isOver = true;

			this->c.Y = this->snake.body[0].y;
			this->c.X = this->snake.body[0].x;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 10);
			putchar(this->snake.headSym);


		}
		string str = to_string(this->snake.points);
		this->statistic = "\t  " + this->name + "        Points: " + str;
		this->c.X = this->width / 2 - 4;
		this->c.Y = this->height / 2;

		SetConsoleCursorPosition(h, c);
		SetConsoleTextAttribute(h, 12);
		cout << "Game Over!";
		while (_getch() != 13) {
			this->c.X = this->width / 2 + 7;
			this->c.Y = this->height / 2;
			SetConsoleCursorPosition(h, c);
			putchar(' ');

		}
		fstream l;
		l.open("d:\\leaderboard.txt", fstream::in | fstream::out | fstream::app);

		while (!l.is_open()) {
			l.open("d:\\leaderboard.txt");
		}

		l << this->statistic << "\n";

		l.close();
		system("taskkill /IM VsDebugConsole.exe");
	}
	void draw() {

		cout << endl;
		SetConsoleTextAttribute(h, 15);
		for (short y = -1; y <= this->height; y++) {
			for (short x = -1; x <= this->width; x++) {
				if ((y == -1 || y == this->height) && x > -1 && x < this->width) cout << (char)196;
				else if ((x == -1 || x == this->width) && y > -1 && y < this->height) cout << (char)179;
				else if (x == -1 && y == -1) cout << (char)218;
				else if (x == -1 && y == this->height) cout << (char)192;
				else if (y == -1 && x == this->width) cout << (char)191;
				else if (y == this->height && x == this->width) cout << (char)217;
				else cout << ' ';
			}
			cout << "\n";
		}
		this->c.X = this->apple.x;
		this->c.Y = this->apple.y;
		SetConsoleCursorPosition(h, c);
		SetConsoleTextAttribute(h, 12);
		putchar(this->appleSym);
	}
};

int main() {
	COORD c;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci = { sizeof(cci), false };

	system("mode con cols=42 lines=24");
	Game g = Game();



	cout << "\n\n\t    \x1b[92m" << (char)15 << (char)15 << (char)15 << (char)15 << " \x1b[0m\x1b[91mSnake Game\x1b[0m\x1b[92m " << (char)15 << (char)15 << (char)15 << "<\x1b[0m\n\n";
	cout << "\t\t   Start\n\n";
	cout << "\t        Leaderboard\n\n";
	cout << "\t\t   Leave";

	c.X = 17;
	c.Y = 4;
	SetConsoleCursorPosition(h, c);
	SetConsoleTextAttribute(h, 12);
	putchar('>');

	c.X = 25;
	c.Y = 4;
	SetConsoleCursorPosition(h, c);
	SetConsoleTextAttribute(h, 12);
	putchar('<');

	char menu = 'q';
	int i = 0;

	char str[100];


	while (menu != 13) {

		menu = _getch();

		if (menu == 's') i++;
		else if (menu == 'w') i--;

		if (i < 0) i = 0;
		else if (i > 2) i = 2;

		switch (i) {
		case 1:
			c.X = 17;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');

			c.X = 25;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');

			c.X = 14;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('>');

			c.X = 28;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('<');

			c.X = 17;
			c.Y = 8;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');

			c.X = 25;
			c.Y = 8;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');
			break;
		case 2:
			c.X = 17;
			c.Y = 8;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('>');

			c.X = 25;
			c.Y = 8;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('<');

			c.X = 14;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');

			c.X = 28;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');
			break;
		case 0:
			c.X = 17;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('>');

			c.X = 25;
			c.Y = 4;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar('<');

			c.X = 14;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');

			c.X = 28;
			c.Y = 6;
			SetConsoleCursorPosition(h, c);
			SetConsoleTextAttribute(h, 12);
			putchar(' ');
			break;
		}

		if (i == 0 && menu == 13) {
			system("cls");

			cout << "\n\n\t    \x1b[92m" << (char)15 << (char)15 << (char)15 << (char)15 << " \x1b[0m\x1b[91mSnake Game\x1b[0m\x1b[92m " << (char)15 << (char)15 << (char)15 << "<\x1b[0m\n\n";
			SetConsoleTextAttribute(h, 12);
			cout << "   [~]Enter player name:\x1b[0m ";
			cin >> g.name;

			g.loop();
		}
		else if (i == 1 && menu == 13) {
			system("cls");

			cout << "\n\n\t   \x1b[92m" << (char)15 << (char)15 << (char)15 << (char)15 << " \x1b[0m\x1b[91mLeaderBoard\x1b[0m\x1b[92m " << (char)15 << (char)15 << (char)15 << "<\x1b[0m\n\n";

			fstream l;
			l.open("d:\\leaderboard.txt", fstream::in | fstream::out | fstream::app);
			while (!l.is_open()) {
				l.open("d:\\leaderboard.txt");
			}
			while (!l.eof()) {
				string s = to_string(i);
				g.statistic = "";
				getline(l, g.statistic);
				cout << g.statistic << endl;
			}
			l.close();

			menu = _getch();

			if (menu == 13) {
				return main();
			}

		}
		else if (i == 2 && menu == 13) { system("taskkill /IM VsDebugConsole.exe"); }
	}

	char _;
	cin >> _;
	return 0;

}