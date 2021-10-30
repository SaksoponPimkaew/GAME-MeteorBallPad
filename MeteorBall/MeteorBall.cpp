#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include <string.h>
constexpr auto screen_x = 80;
#define screen_y 58
#define specialbox 5
#define starcount 45
HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
int starbegin = 0;

FILE* mptr;
int i;
struct star
{
	int x=0;
	int y=0;
}stars[starcount];
struct player {
	char name[50] = "";
	int times = 0;
	int score = 0;
}p[10];

struct bullet {
	int x = 0;
	int y = 0;
	bool status = 0;
}bullets[5];
struct defender {
	int x = 0;
	int y = screen_y - 2;
	int hp = 3;
}defenders[screen_x];
struct block
{
	int x=0;
	int y=0;
	int hp = 0;
	int brick = 0;
	int color = 0;

}blocks[200 + specialbox];
struct bal
{
	int x = 0;;
	int y = screen_y - 34;
	int xstatus = 1;
	int ystatus = 1;

}balls[3];
struct ship
{
	int x;
	int y;

}sh[5][5];

void setcolor(int fg, int bg);
void setcursor(bool visible);
int setMode();
char cursor(short x, short y);
void gotoxy(short x, short y);
int setConsole(int x, int y);
void clearall_buffer();
void fill_star();
void fill_buffer_to_console();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	setConsole(screen_x, screen_y);
	setMode();
	while (1)
	{
		fill_star();
		fill_buffer_to_console();
		Sleep(200);
		clearall_buffer();
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}
void gotoxy(int x,int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}
void clearall_buffer(void) {
	for (int i = 0; i < screen_x; i++)
	{
		for (int j =0; j < screen_y; j++)
		{
			consoleBuffer[i+80 * j].Char.AsciiChar = ' ';
			
		}
	}
}
void fill_star() {
	
	if (starbegin !=0)
	{
		for (int i = 0; i < starcount; i++)
		{
			if (stars[i].y>=screen_y-1)
			{
				if (cursor(stars[i].x + 80 , stars[i].y)!= '*' )
				{
					
					stars[i].x = rand()%80;
					stars[i].y = 3;
				}
				else i--;
			}
			else stars[i].y +=1 ;
		}
	}

	if (starbegin==0)
	{
		for (int i = 0; i < starcount; i++)
		{
			if (cursor(stars[i].x + 80, stars[i].y) == '*' || cursor(stars[i].x + 80, stars[i].y + 1) != '*')
			{
				stars[i].x = (rand() % screen_x);
				stars[i].y = 1 + (rand() % 45);
			}
			else i--;
		}
		
	}
	starbegin += 1;
	for (int i = 0; i < starcount; i++)
	{
		
		consoleBuffer[stars[i].x+80  * stars[i].y].Char.AsciiChar = '*';
		consoleBuffer[stars[i].x+80  * stars[i].y].Attributes = 2;
	}
}
