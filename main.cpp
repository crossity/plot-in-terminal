#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define HEIGHT 23  // write your Height of selected Screen
#define WIDTH  80  // write your Width  of selected Screen

using namespace std;

struct POINT {
	int x, y;
	string name;
	POINT (int a, int b, string c) {
		x = a; y = b; name = c;
	}
	POINT () {
		x = 0; y = 0; name = "NULL";
	}
};

struct LINE {
	POINT p[2];
	LINE (POINT a, POINT b) {
		p[0] = a;
		p[1] = b;
	}
};

float ABS(float a) {
	if (a < 0)
		return a * -1;
	else 
		return a;
}

bool findchar(string s, char c);

vector<string> split(string s, char c);

POINT FindPoint (vector<POINT> points, string a);

void ClearScreen(char c);
void ShowScreen();

void DrawLine(POINT a, POINT b, char c);
void DrawLineUp(POINT a, POINT b, char c);
void DrawPoint(POINT a, char c);

char SCREEN[HEIGHT][WIDTH];

int main() {
	ClearScreen('.');
	int count = 0;
	for (int i = 0; i < HEIGHT; i += 2) {
		SCREEN[i][0] = count / 10 + 48;
		SCREEN[i][1] = count % 10 + 48;
		count++;
	}
	count = 0;
	for (int i = 0; i < WIDTH; i += 3) {
		SCREEN[0][i] = count / 10 + 48;
		SCREEN[0][i + 1] = count % 10 + 48;
		SCREEN[0][i + 2] = ' ';
		count++;
	}
	ifstream input("input.txt");
	string line = "";
	int mode = 0;
	int LinesCount = 0;
	int LineOn = 0;
	vector<POINT> points;
	while (getline(input, line)) {
		if (mode == 0) { // 0 - points
			if (!findchar(line, ' ')) {
				LinesCount = stoi(line);
				mode = 1;
			}
			else {
				vector<string> words;
				words = split(line, ' ');
				points.push_back(POINT(stof(words[1]), stof(words[2]), words[0]));
			}
		}
		else if (mode == 1) { // 1 - lines between
			if (LineOn == LinesCount + 1)
				break;
			vector<string> words = split(line, ' ');
			for (int i = 0; i < words.size() - 1; i++)
				cout << words[i] << ", ";
			cout << words[words.size() - 1] << endl;
			DrawLine(FindPoint(points, words[0]), FindPoint(points, words[1]), '*');
			LineOn++;
		}
	}
	ShowScreen();
	return 0;
}

bool findchar(string s, char c) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == c)
			return true;
	}
	return false;
}

vector<string> split(string s, char c) {
	string word;
	vector<string> words;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == c) {
			if (word != "")
				words.push_back(word);
			word = "";
		}
		else
			word += s[i];
		if (i == s.size() - 1) {
			if (word != "") {
				words.push_back(word);
			}
			word = "";
		}
	}
	return words;
}

POINT FindPoint (vector<POINT> points, string a) {
	for (int i = 0; i < points.size(); i++) {
		if (a == points[i].name)
			return points[i];
	}
	return POINT(0, 0, "f");
}

void ShowScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cout << SCREEN[y][x];
		}
		cout << endl;
	}
}

void ClearScreen(char c) {
	for (int y = 0; y < HEIGHT; y += 2) {
		for (int x = 0; x < WIDTH; x += 3) {
			SCREEN[y][x] = c;
			SCREEN[y][x + 1] = ' ';
			SCREEN[y][x + 2] = ' ';
			SCREEN[y + 1][x] = ' ';
			SCREEN[y + 1][x + 1] = ' ';
			SCREEN[y + 1][x + 2] = ' ';
		}
	}
}

void DrawPoint(POINT a, char c) {
	if (a.x < WIDTH && a.x >= 0 && a.y < HEIGHT && a.y >= 0)
		SCREEN[(int)a.y][(int)a.x] = c;
}

void DrawLineUp(POINT a, POINT b, char c) {
	int mov = (a.y < b.y) * 2 - 1;
	while ((int)a.y != (int)b.y) {
		DrawPoint(a, c);
		a.y += mov;
	}
}

void DrawLine(POINT A, POINT B, char c) {
	float k, b;
	float x1, y1, x2, y2;
	if (A.x < B.x) {
		x1 = A.x * 3;
		x2 = B.x * 3;
		y1 = A.y * 2;
		y2 = B.y * 2;
	}
	else {
		x1 = B.x * 3;
		x2 = A.x * 3;
		y1 = B.y * 2;
		y2 = A.y * 2;
	}
	if (x1 != x2) {
		k = (y1 - y2) / (x1 - x2);
		b = y1 - k * x1;
	}
	else {
		DrawLineUp(POINT(x1, y1, "d"), POINT(x2, y2, "d"), c);
		return;
	}
	cout << "k = " << k << endl;
	cout << "b = " << b << endl;
	DrawPoint(POINT(x1, y1, "d"), c);
	DrawPoint(POINT(x2, y2, "d"), c);
	POINT Old(x1, y1, "a");
	while (!((int)x1 >= (int)x2 && (int)y1 >= (int)y2)) {
		x1 += 1;
		y1 = k * x1 + b;
		DrawPoint(POINT(x1, y1, "d"), c);
		DrawLineUp(Old, POINT(x1, y1, "d"), c);
		Old.x = x1;
		Old.y = y1;
 	}
}
