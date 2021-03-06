#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include <string.h>
#include <cwchar>
constexpr auto screen_x = 80;
#define screen_y 58
#define specialbox 5
#define starcount 45
HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
DWORD numEvents = 0;
DWORD numEventsRead = 0;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
int power = 0;
int starbegin = 0, score =0, times =0, timess=0,buff=1,r=2,timesx=0;
int posx = 0, posy = 0,position=0;
int lv = 1, balllv = 1, ballcount = 1, bc = 0,delay =0;
int random=1,randomcharm = 2;
int lvcache = 0, ballcache = 0,balllvcache =0, delaycache = 0, timescache =0;
bool play = 1, sub = 1, over=0;
int adjusttime = 0;
int charge =0;
int displaycolorful = 0;
int menuposx = 0, menuposy = 0;
FILE* mptr;
int  defendersetup = 0, blocksetup=0;
char name[30] = "";
int padhelper[5] = { 10, 11, 12, 13, 14 };
int padhelperstatus = 1;
bool goStatus = 0, topStatus =0, exitStatus =0;
int gocolor = 15;
int namecount = 0,namecolor1=0,namecolor2;
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
	int8_t x = 0;
	int y = 0;
	bool status = 0;
}bullets[5];

struct block
{
	short x=0;
	short y=0;
	short hp = 0;
	short color = 0;

}blocks[300 + specialbox];
struct bal
{
	int x = 0;;
	int y = screen_y - 34;
	int xstatus = 1;
	int ystatus = 1;
	int status = 0;
}balls[3];
struct ship
{
	int x;
	int y;

}sh[5][9];

struct defender
{
	int x;
	int y ;
	int hp;
}defenders[screen_x];
struct bigball
{
	int x[2] = { 0,1 };
	int y[2] = { 0,1 };
	int xstatus = 1;
	int ystatus = 1;
	int status = 0;
}bigballs[3];
struct angel
{
	int x = 10;
	int y = 5;
	int status = 0;

}angels;
struct devil
{
	int x=5;
	int y=5;
	int hp = 5;
	int status =0;
}devils;
struct item {
	int x=0;
	int y=0;
	int lv = 0;
}items;
struct godbullet
{
	int x=0;
	int y=0;
	int status=0;

}godbullets;
struct Ultimate
{
	int x=0;
	int y=0;
	bool status=1;
}ultimate[screen_x][5];
void setcolor(int fg, int bg);
void setcursor(bool visible);
int setMode();
char cursor(short x, short y);
void gotoxy(short x, short y);
int setConsole(int x, int y);
void clearall_buffer();
void fill_star();
void fill_buffer_to_console();
void fill_defender_to_console();
void fill_blocks_to_console();
void reset_blocks();
void write_console(int x, int y, int color, char ascii);
void scoreSetup();
void nameSetup();
void userinterface();
void inputUpdate();
void TOP5();
void displaybuff(int timess, int& buff);
void displayname(int timess);
void displayHP(int timess);
void displayscore(int& score);
void fill_ship_to_buffer(int x, int y);
void ball_move();
void bullet_move();
void sounds(int song);
void summonAD();
void itemdrop(int angelstatus, int devilstatus);
void padhelp();
void bullmove();
void adjustpow(int &adjusttime);
void ultimate_power_set();
void ultimate_reset();
void ultimate_burst();
void mainmenu();
void display_percent();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	srand(time(NULL));
	setcursor(0);
	setConsole(screen_x, screen_y);
	//setMode();
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	mainmenu();
	if (over)
		return 0;
	clearall_buffer();
	fill_buffer_to_console();
	nameSetup();
	setMode();
	scoreSetup();

	while (play)
	{

		padhelp();
		inputUpdate();
		fill_star();
		display_percent();
		fill_defender_to_console();
		fill_blocks_to_console();
		//inputUpdate();
		displaybuff(timess, buff);
		displayname( timess);
		displayHP(timess);
		displayscore(score);
		fill_ship_to_buffer(posx, screen_y - 3 + position);
		bullet_move();
		summonAD();
		
		itemdrop(angels.status, devils.status);
		bullmove();
		ball_move();
		ultimate_power_set();
		fill_buffer_to_console();
		adjustpow(adjusttime);
 		ultimate_burst();
		if (delay <= 4000)
		{
			delay += 1;
		}
		if (times>=10)
		{
			userinterface();
			reset_blocks();
			displaycolorful = 0;
			angels.status = 0;
			devils.status = 0;
		}
		clearall_buffer();
		if (over)
			return 0;
		Sleep(55 - delay / 200);
		timesx++;

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
char cursor(short x,short y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}
void gotoxy(short x,short y)
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
			consoleBuffer[i+ screen_x * j].Char.AsciiChar = ' ';
			consoleBuffer[i + screen_x * j].Attributes = 0;
		}
	}
}
void fill_star() {
	
	if (starbegin !=0)
	{
		for (int i = 0; i < (delay / 100); i++)
		{
			if (stars[i].y>=screen_y-1)
			{
				if (cursor(stars[i].x + screen_x , stars[i].y)!= '*' )
				{
					stars[i].x = rand()% screen_x;
					stars[i].y = 3+rand()%20;
				}
				else i--;
			}
			else {
				stars[i].y += 1;
				for (int j = 0; j < 5 + (4 * ((lv + 1) / (2 + (lv / 3)))); j++)
				{
					if ((lv == 0 ) && ((stars[i].x == sh[lv][j].x) && stars[i].y == sh[lv][j].y)) {

						times++;
						stars[i].x = rand() % screen_x;
						stars[i].y = 3 + rand() % 20;
						sounds(2);
					}
					else if ((lv >= 1 && ((stars[i].x == sh[lv][j].x) && stars[i].y == sh[lv][j].y))) {

						times++;
						stars[i].x = rand() % screen_x;
						stars[i].y = 3 + rand() % 20;
						sounds(2);
					}

				}
				for (int t = 0; t <3; t++)
				{
					if (balls[t].status==1)
					{
						if (balls[t].x==stars[i].x && balls[t].y==stars[i].y)
						{
							score += 10;
							stars[i].x = rand() % screen_x;
							stars[i].y = 3 + rand() % 20;
						}
					}
				}


			}
		}
	}

	if (starbegin==0)
	{
		for (int i = 0; i <(delay / 100); i++)
		{
			if (cursor(stars[i].x + screen_x, stars[i].y) == '*' || cursor(stars[i].x + 80, stars[i].y + 1) != '*')
			{
				stars[i].x = (rand() % screen_x);
				stars[i].y = 1 + (rand() % 45);
			}
			else i--;
		}
		
	}
	starbegin += 1;
	for (int i = 0; i <  (delay / 100); i++)
	{
		write_console(stars[i].x, stars[i].y, 1, '*');
	}
}
void fill_defender_to_console() {
	if (defendersetup==0)
	{
		for (int i = 0; i < screen_x; i++)
		{
			defenders[i].x = i;
			defenders[i].y = screen_y-2;
			defenders[i].hp = 3;
		}
		defendersetup++;
	}
	for (int i = 0; i < screen_x; i++)
	{
		if (defenders[i].hp==3)
		{
			write_console(defenders[i].x, defenders[i].y, 2,240);
		}
		else if (defenders[i].hp == 2)
		{
			write_console(defenders[i].x, defenders[i].y, 14, 240);
		}
		else if (defenders[i].hp ==1)
		{
			write_console(defenders[i].x, defenders[i].y, 4, 240);
		}
		else write_console(defenders[i].x, defenders[i].y, 0, ' ');
	}

}
void reset_blocks() {
	for (int i = 0; i < 300 + specialbox; i++)
	{
		if (i < 300 + specialbox)
		{
			blocks[i].hp = 0;
		}
	}
}
void fill_blocks_to_console() {
	

		for (int i = 0; i < 300+specialbox; i++)
		{
			if (i<300+specialbox && blocks[i].hp==0)
			{
				blocks[i].x = rand() % screen_x;
				blocks[i].y =4+ rand() %(screen_y-35);
				blocks[i].hp = 1 + rand() % 3;
				blocks[i].color = rand() % 14;
				for (int j = 0; j < i; j++)
				{
					if (blocks[j].x==blocks[i].x)
					{
						if (blocks[j].y == blocks[i].y)
						{
							i--;
							break;
						}
					}
				}
			}
		}

	for (int i = 0; i < 300+specialbox; i++)
	{
		if (i<300)
		{
			if (blocks[i].hp==3)
			{
				write_console(blocks[i].x, blocks[i].y, blocks[i].color, '~');
			}
			else if (blocks[i].hp == 2)
			{
				write_console(blocks[i].x, blocks[i].y, blocks[i].color, '=');
			}
			else if (blocks[i].hp == 1)
			{
				write_console(blocks[i].x, blocks[i].y, blocks[i].color, '-');
			}

			
		}
		else if (i>=300)
		{
			write_console(blocks[i].x, blocks[i].y, blocks[i].color+(rand()%240), 'S');
		}
	}

}
void write_console(int x, int y, int color, char ascii) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ascii;
	consoleBuffer[x + screen_x * y].Attributes = color;

}
void scoreSetup() {
	mptr = fopen("data2.txt", "r");
	for (int i = 0; i < 5; i++)
	{
		fscanf(mptr, "%s %d %d\n", &p[i].name, &p[i].times, &p[i].score);
	}
	for (int i = 0; i < 5; i++)
	{
		printf("%s ", p[i].name);
		printf("%ds ", p[i].times);
		printf("%d\n", p[i].score);
	}
	fclose(mptr);
}
void nameSetup() {
	setcursor(1);
	for (int i = 27; i < 51; i++)
	{
		for (int j = 24; j < 35; j++)
		{
			gotoxy(i, j);
		 printf(" ");

		}
	}

	char entername[] = { 'E','n','t','e','r',' ','Y','o','u','r',' ','N','i','c','k','n','a','m','e',':' };
	

	for ( namecount = 0; namecount < 25; namecount++)
	{
		namecolor1 = 15+(rand() % 7) * 16;
		gotoxy(48 + (namecount / 2), 27);
		for (int i = 0; i < 20; i++)
		{
			consoleBuffer[27 + i - (namecount / 2) + screen_x * 27].Char.AsciiChar = entername[i];
			consoleBuffer[27 + i - (namecount / 2) + screen_x * 27].Attributes = namecolor1;
		}
		for (int i = 0; i < namecount; i++)
		{
			consoleBuffer[48 + i - (namecount / 2) + screen_x * 27].Char.AsciiChar = name[i];
			consoleBuffer[48 + i - (namecount / 2) + screen_x * 27].Attributes = namecolor1;
		}
		fill_buffer_to_console();
		char h = 8;
		scanf("%c", &name[namecount]);
		clearall_buffer();
		if (name[namecount] == ' ' ) {
			name[namecount] = '\0';
			gotoxy(24, 30);
			
			system("pause");
			break;
		}
		if (name[namecount] == h && namecount >= 0) {
			namecount -= 2;
			if(namecount<=0)
			namecount = -1;
			printf(" ");
			if (namecount == -2) namecount = -1;
		}
		else {


		}

	}
	setcursor(0);

}
void userinterface() {	
	if (times < 10)
	{
		setcolor(15, 0);
	}
	else setcolor(4, 0);
	for (int i = 27; i < 51; i++)
	{
		for (int j = 24; j < 32; j++)
		{
			gotoxy(i, j);
			if (j==24 || j==31)
			{
				printf("=");
			}
			else printf(" ");
			
		}
	}
	gotoxy(34, 25);
	
	printf("<<Pause>>");
	if (times >=10)
	{
		sounds(4);
		gotoxy(32, 25);
		charge = 0;
		printf("<<Game Over>>");
		gotoxy(28, 26);
		printf("SPACE    >>  Play Again");
		
		for (int i = 0; i <= 4; i++)
		{
			if (score > p[i].score) {
				p[i + 5].score = p[i + 4].score;
				p[i + 4].score = p[i + 3].score;
				p[i + 3].score = p[i + 2].score;
				p[i + 2].score = p[i + 1].score;
				p[i + 1].score = p[i].score;
				p[i].score = score;
				p[i + 5].times = p[i + 4].times;
				p[i + 4].times = p[i + 3].times;
				p[i + 3].times = p[i + 2].times;
				p[i + 2].times = p[i + 1].times;
				p[i + 1].times = p[i].times;
				p[i].times = timesx;
				strcpy(p[i + 5].name, p[i + 4].name);
				strcpy(p[i + 4].name, p[i + 3].name);
				strcpy(p[i + 3].name, p[i + 2].name);
				strcpy(p[i + 2].name, p[i + 1].name);
				strcpy(p[i + 1].name, p[i].name);
				strcpy(p[i].name, name);
				break;
			}

		}
		mptr = fopen("data2.txt", "w");
		for (int i = 0; i < 5; i++)
		{
			fprintf(mptr, "%s ", p[i].name);
			fprintf(mptr, "%d ", p[i].times);
			fprintf(mptr, "%d\n", p[i].score);
		}
		fclose(mptr);
		score = 0;
		delay = 0;
		lv = 2;
		power = 0;
		times = 0;
		timesx = 0;
		ultimate_reset();
		defendersetup = 0;
		ballcount = 1;
	}
	else {
		gotoxy(28, 26);
		printf("SPACE    >>   Play");
	}
	gotoxy(28, 27);
	printf("A        >>Hall of Fame");
	gotoxy(28, 28);
	printf("S        >>  Rename");
	gotoxy(28, 29);
	printf("X        >> MainMenu");
	gotoxy(28, 30);
	printf("ESC      >>   Exit");
	setcursor(0);
	char ch = 'j';
	do {

		if (_kbhit()) {

			ch = _getch();
			if (ch == 27 ) {
				gotoxy(0, 0);
				play = 0;
			}

			if (ch == 'a') {
				TOP5();
			}
			if (ch == 's') {
				nameSetup();
			}
			if (ch == 'x')
				sub = 1; 
				mainmenu();
			fflush(stdin);
		}
		setMode();
	} while (ch == 'j');

}

void inputUpdate() {
	GetNumberOfConsoleInputEvents(rHnd, &numEvents);
	if (numEvents != 0) {
		INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
		ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
		for (DWORD i = 0; i < numEventsRead; ++i) {
			if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == 1) {
				if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && sub) {
					sub = 0;
					over = 1;

				}
				else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
					userinterface();
				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
				{
					power = (power + 1) % 3;
				}

				if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'w') {
					if (position >= -5)
					{
						position -= 1;
					}

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 's') {
					if (position <= -1)
					{
						position += 1;
					}

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a'&& sub) {
					TOP5();

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a') {
					ballcount = 1 + (ballcount) % 3;

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'z') {

					lv = (lv + 1) % 4;

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'z') {
					r = 1 + rand() % 15;


				}
				else if ((eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'x') && timesx - charge > 750) {
					ultimate_reset();
					charge = timesx;
					displaycolorful = 0;
				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == ' ' && sub)
					sub = !sub;
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == ' ')
				{
					if (position <= -1)
					{
						position += 1;

					}
					if (lv == 2 && bullets[0].status == 0) {

						bullets[0].x = sh[lv][4].x;
						bullets[0].y = screen_y - 4 + position;
						bullets[0].status = 1;
						std::thread bomb3(Beep, 950, 120);
						bomb3.detach();
						std::thread bomb4(Beep, 750, 120);
						bomb4.detach();


					}
					else if (lv == 3 && bullets[1].status == 0) {

						bullets[1].x = sh[lv][4].x;
						bullets[1].y = screen_y - 3 + position;
						bullets[1].status = 1;

					}
				}
			}
			 if (eventBuffer[i].EventType == MOUSE_EVENT) {

				posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
				posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
				if ((posx > 9 && posx < 22) && (posy > 19 && posy < 24))  goStatus = 1;
				else goStatus = 0;
				if ((posx > 27 && posx < 53) && (posy > 29 && posy < 34))  topStatus = 1;
				else topStatus = 0;
				if ((posx > 47 && posx < 72) && (posy > 41 && posy < 47)) exitStatus = 1;
				else exitStatus = 0;

				if ((eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)&&sub) {
					if((posx>9 && posx<22)&&( posy>19 && posy<24))
					sub = !sub;
					if ((posx > 27 && posx < 53) && (posy > 29 && posy < 34))  TOP5();
					if ((posx > 47 && posx < 72) && (posy > 41 && posy < 47)) {
						sub = 0;
						over = 1;
					}
				}
				else if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (position >= -5)
					{
						position -= 1;
					}
				}
				else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
					RIGHTMOST_BUTTON_PRESSED) {

					if (position <= -1)
					{
						position += 1;

					}
					if (lv == 2 && bullets[0].status == 0 ) {

						bullets[0].x = sh[lv][4].x;
						bullets[0].y = screen_y - 4 + position;
						bullets[0].status = 1;
						std::thread bomb3(Beep, 950, 120);
						bomb3.detach();
						std::thread bomb4(Beep, 750, 120);
						bomb4.detach();


					}
					else if (lv == 3 && bullets[1].status == 0 ) {

						bullets[1].x =sh[lv][4].x;
						bullets[1].y = screen_y - 3 + position;
						bullets[1].status = 1;

					}

				}
				else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & SBM_GETSCROLLINFO) {
					r = 1 + rand() % 14;
				}
			}
		}
		delete[] eventBuffer;
	}
}

void TOP5() {
	int to[] = { 223,223,223,223,223,' ', 223,223,223,223,' ',223,223,223,' ' ,' ',' ',' ',' ',223,223,223,223 ,' ',219 };
	int oo[] = { ' ',' ',223,' ',' ',' ', 223 , ' ',' ', 223,' ', 219 ,' ',' ',223,' ',' ',' ',' ',223, ' ',' ',' ',' ',219 };
	int po[] = { ' ',' ',223,' ',' ' ,' ', 223, ' ',' ', 223,' ',219,223,223,223,' ',' ',' ',' ',223,223, 223,223, ' ', 219 };
	int fo[] = { ' ',' ',223,' ',' ' ,' ', 223,223,223,223,' ', 219 ,' ',' ',' ',' ',' ',' ',' ',220,220,220,223,' ',220 };
	int playposition = 12;
	clearall_buffer();
	fill_buffer_to_console();
	setcolor(15, 0);
	for (int i = 0; i < 25; i++)
	{
		consoleBuffer[playposition + 18 + i + screen_x * 18].Char.AsciiChar = to[i];
		consoleBuffer[playposition + 18 + i + screen_x * 18].Attributes = gocolor;
		consoleBuffer[playposition + 18 + i + screen_x * 19].Char.AsciiChar = oo[i];
		consoleBuffer[playposition + 18 + i + screen_x * 19].Attributes = gocolor;
		consoleBuffer[playposition + 18 + i + screen_x * 20].Char.AsciiChar = po[i];
		consoleBuffer[playposition + 18 + i + screen_x * 20].Attributes = gocolor;
		consoleBuffer[playposition + 18 + i + screen_x * 21].Char.AsciiChar = fo[i];
		consoleBuffer[playposition + 18 + i + screen_x * 21].Attributes = gocolor;
	}
	fill_buffer_to_console();
	gotoxy(38, 22);
	printf("\n");
	mptr = fopen("data2.txt", "r");
	for (int i = 0; i < 5; i++)
	{
		fscanf(mptr,"%s %d %d\n", &p[i].name, &p[i].times, &p[i].score);
	}
	for (int i = 0; i < 5; i++)
	{
		gotoxy(35-strlen(p[i].name)/2, 24+2*i);
		printf("%s ", p[i].name);
		printf("%ds ", p[i].times);
		printf("%d\n", p[i].score);
	}
	fclose(mptr);
	setcursor(1);
	gotoxy(26, 24 + 10);
	system("pause");
	setcursor(0);
	clearall_buffer();
	fill_buffer_to_console();
	userinterface();
}
void displayscore(int& score) {
	int  digit = score;
	char sdisplay;

	timess = 0;
	for (int i = 0; i < 10; i++)
	{


		if (i >= 1) {
			timess += 1;

			int tempdigit2 = digit % 10;
			sdisplay = 48 + tempdigit2;
			digit = digit / 10;
			consoleBuffer[80 - timess + screen_x * 0].Char.AsciiChar = sdisplay;
			consoleBuffer[80 - timess + screen_x * 0].Attributes = 18;
		}
		if (digit / 10 < 1) {
			char sdisplay2 = 48 + digit;
			consoleBuffer[79 - timess + screen_x * 0].Char.AsciiChar = 48 + digit;
			consoleBuffer[79 - timess + screen_x * 0].Attributes = 18;
			i = 10;
		}

	}

	consoleBuffer[73 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[73 - timess + screen_x * 0].Char.AsciiChar = 'S';
	consoleBuffer[74 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[74 - timess + screen_x * 0].Char.AsciiChar = 'C';
	consoleBuffer[75 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[75 - timess + screen_x * 0].Char.AsciiChar = 'O';
	consoleBuffer[76 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[76 - timess + screen_x * 0].Char.AsciiChar = 'R';
	consoleBuffer[77 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[77 - timess + screen_x * 0].Char.AsciiChar = 'E';
	consoleBuffer[78 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[78 - timess + screen_x * 0].Char.AsciiChar = '=';


}
void displaybuff(int timess, int& buff) {

	if (buff == 1 && ballcount == 3)
	{

		consoleBuffer[70 - timess + screen_x * 0].Char.AsciiChar = 'T';
	}
	if (buff == 1 && ballcount == 2)
	{
		consoleBuffer[70 - timess + screen_x * 0].Char.AsciiChar = 'D';
	}
	if (buff == 1 && ballcount == 1)
	{
		consoleBuffer[70 - timess + screen_x * 0].Char.AsciiChar = 'N';
	}
	consoleBuffer[70 - timess + screen_x * 0].Attributes = 19+((power)*1*(power+2)) ;
	consoleBuffer[55-lv + (times / 2) - strlen(name) - timess + screen_x * 0].Char.AsciiChar = 'P';
	consoleBuffer[55-lv +( times / 2) - strlen(name) - timess + screen_x * 0].Attributes = 18;
	if (lv >= 0)
	{

		consoleBuffer[56 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Char.AsciiChar = '/';
		consoleBuffer[56 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Attributes = 18;
	}
	if (lv >=1)
	{

		consoleBuffer[57 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Char.AsciiChar = '/';
		consoleBuffer[57 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Attributes = 19 ;
	}
	if (lv>= 2)
	{

		consoleBuffer[58 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Char.AsciiChar = '/';
		consoleBuffer[58 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Attributes = 23;
	}
	if (lv== 3)
	{

		consoleBuffer[59 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Char.AsciiChar = '/';
		consoleBuffer[59 - lv + (times / 2) - strlen(name) - timess + screen_x * 0].Attributes = 21;
	}
	

}
void displayname(int timess) {
	char* namecopy = name;
	for (int i = 0; i < int(strlen(name)); i++)
	{
		consoleBuffer[70 - strlen(name) - 2 - timess + i + i * 0].Char.AsciiChar = name[i];
		consoleBuffer[70 - strlen(name) - 2 - timess + i + i * 0].Attributes = 18;
	}
}

void displayHP(int timess) {
	consoleBuffer[70 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 'H';
	consoleBuffer[70 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
	consoleBuffer[71 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 'P';
	consoleBuffer[71 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;

	for (int i = 0; i < 5; i++)
	{
		char d = (char)(3);
		gotoxy(70 - strlen(name) + i - 2 - 7 + times / 2 - timess - 1, 0);
		if (times <= 1)
		{
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = d;
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
		}
		else if (times <= 3)
		{
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = d;
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Attributes = 20;
			if (i == 3)
			{
				break;
			}
		}
		else if (times <= 5)
		{
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = d;
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Attributes = 21;
			if (i == 2)
			{
				break;
			}
		}
		else if (times <= 7)
		{
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = d;
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Attributes = 22;
			if (i == 1)
			{
				break;
			}
		}
		else if (times <= 9)
		{
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = d;
			consoleBuffer[70 - strlen(name) + i - 2 - 7 + times / 2 - timess + screen_x * 0].Attributes = 23;
			if (i == 0)
			{
				break;
			}
		}
		else if (times == 10)
		{
			setcolor(0, 1);
			printf("X");
			if (i == 0)
			{
				break;
			}

		}
	}


}
void fill_ship_to_buffer(int posx, int posy)
{

	for (int i = 0; i < 5+(4*((lv+1)/(2+(lv/3)))); i++)
	{
		if (lv == 0 && posx <= screen_x -3 && posx > 4){
			
			sh[lv][i] = { posx + i-2,posy };
		}
		else if (lv >= 1 && posx >  6 && posx <= screen_x - 5) {
			
			sh[lv][i] = { posx + i-4,posy };
		}
		else if (lv ==0)
		{
			if (posx < 5) {
				posx = 4;
			}
			sh[lv][i] = { posx + i - 4,posy };
		}
		else if (lv>0)
		{
			if (posx > screen_x - 7) {
				posx = screen_x - 5;
			}
			else posx = 4;
			sh[lv][i] = { posx + i - 4,posy };
		}
		consoleBuffer[sh[lv][i].x + screen_x * sh[lv][i].y].Attributes = r;
	}


	if (lv == 0)
	{
		consoleBuffer[sh[0][0].x + screen_x * sh[lv][0].y].Char.AsciiChar = '-';

		consoleBuffer[sh[0][1].x + screen_x * sh[0][1].y].Char.AsciiChar = '-';
		consoleBuffer[sh[0][2].x + screen_x * sh[0][2].y].Char.AsciiChar = '-';
		consoleBuffer[sh[0][3].x + screen_x * sh[0][3].y].Char.AsciiChar = '-';
		consoleBuffer[sh[0][4].x + screen_x * sh[0][4].y].Char.AsciiChar = '-';
	}
	if (lv == 1) {
		for (int i = 0; i < 9; i++)
		{
			consoleBuffer[sh[1][i].x + screen_x * sh[1][i].y].Char.AsciiChar = '-';
		}
	}
	if (lv == 2) {
		consoleBuffer[sh[2][0].x + screen_x * sh[2][0].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][1].x + screen_x * sh[2][1].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][2].x + screen_x * sh[2][2].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][3].x + screen_x * sh[2][3].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][4].x + screen_x * sh[2][4].y].Char.AsciiChar = '^';
		consoleBuffer[sh[2][5].x +  + screen_x * sh[2][5].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][6].x  + screen_x * sh[2][6].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][7].x  + screen_x * sh[2][7].y].Char.AsciiChar = '-';
		consoleBuffer[sh[2][8].x  + screen_x * sh[2][8].y].Char.AsciiChar = '-';
	}
	if (lv == 3) {
		consoleBuffer[sh[3][0].x + screen_x * sh[3][0].y].Char.AsciiChar = '<';
		consoleBuffer[sh[3][1].x + screen_x * sh[3][1].y].Char.AsciiChar = '-';
		consoleBuffer[sh[3][2].x + screen_x * sh[3][2].y].Char.AsciiChar = '^';
		consoleBuffer[sh[3][3].x + screen_x * sh[3][3].y].Char.AsciiChar = '-';
		consoleBuffer[sh[3][4].x + screen_x * sh[3][4].y].Char.AsciiChar = '^';
		consoleBuffer[sh[3][5].x  + screen_x * sh[3][5].y].Char.AsciiChar = '-';
		consoleBuffer[sh[3][6].x  + screen_x * sh[3][6].y].Char.AsciiChar = '^';
		consoleBuffer[sh[3][7].x  + screen_x * sh[3][7].y].Char.AsciiChar = '-';
		consoleBuffer[sh[3][8].x  + screen_x * sh[3][8].y].Char.AsciiChar = '>';
	}
}
void ball_move() {
	for (int t = 0; t < ballcount; t++)
	{
		for (int b= 0; b < 5; b++)
		{
			if ((balls[t].x == padhelper[b] && balls[t].ystatus==1&& balls[t].y==screen_y-3))
			{
				sounds(1);
				balls[t].xstatus = rand() % 2;
				balls[t].ystatus = (balls[t].ystatus + 1) % 2;
			}
		}
		for (int i = 0; i < 300+specialbox; i++)
		{

			if (power == 1)
			{

				if (balls[t].x == blocks[i].x && balls[t].y == blocks[i].y)
				{
					
					blocks[i].hp--;
					score++;
					sounds(0);
				}

			}
			else if (power == 2) {
				if (balls[t].x == blocks[i].x && balls[t].y == blocks[i].y)
				{
					score += blocks[i].hp;
					blocks[i].hp = 0;
					sounds(0);
				}

			}
			else if (power == 0) {
				if (balls[t].x == blocks[i].x && balls[t].y == blocks[i].y)
				{
					score++;
					balls[t].xstatus = rand() % 2;
					balls[t].ystatus = (balls[t].ystatus + 1) % 2;
					blocks[i].hp--;
					sounds(0);
				}
				
			}

		}
		for (int i = 300; i < 300 + specialbox; i++)
		{
			if (balls[t].x == blocks[i].x && balls[t].y == blocks[i].y)
			{
				sounds(1);
				score += 500;
				times = 0;
				int k = 0;
				k = rand() % 5;
				if (k == 0)
				{
					times = 0;
				}
				else if (k == 1)
				{
					if (lv < 3)
					{
						lv++;
					}
					else score += 1500;
				}
				else if (k == 2)
				{
					if (power < 2)
					{
						power++;
					}
					else score += 1500;
				}
				else if (k == 3)
				{
					score += 1500;
				}
				else if (k == 4) {
					defendersetup = 0;
				}
				blocks[i].hp--;
			}
		}
		for (int i = 0; i < screen_x; i++)
		{
			if ((balls[t].x == defenders[i].x && balls[t].y == defenders[i].y)&& balls[t].ystatus==1&& defenders[i].hp>=1) {
				defenders[i].hp--;
				balls[t].ystatus = (balls[t].ystatus + 1) % 2;
				sounds(0);
			}
		}
		for (int i = 0; i < (delay / 100); i++)
		{
			if ((balls[t].x == stars[i].x && balls[t].y == stars[i].y)) {
				if (balls[t].status == 1)
				{
					score += 10;
					sounds(3);
				}
			}
		}
		if (balls[t].x>=0 && balls[t].xstatus==1 && balls[t].x <screen_x-1)
		{
			balls[t].x++;

		}
		else if (balls[t].x<=screen_x-1 && balls[t].status==0 && balls[t].x>0)
		{
			balls[t].x--;
		}	
		balls[t].y = balls[t].y + (-1) + (2 * balls[t].ystatus);
		if (balls[t].x==0 || balls[t].x ==screen_x-1)
		{
			balls[t].xstatus = (balls[t].xstatus + 1) % 2;
			sounds(0);
		}
		if (balls[t].y == 0 )
		{
			balls[t].ystatus = (balls[t].ystatus + 1) % 2;
			sounds(0);
		}
		else if(balls[t].y == screen_y-1)
		{
			balls[t].ystatus = (balls[t].ystatus + 1) % 2;
			times++;
			sounds(2);
		}
		for (int i = 0; i < 5 + (4 * ((lv + 1) / (2 + (lv / 3)))); i++)
		{
			if ((lv == 0 && balls[t].ystatus==1) && ((balls[t].x== sh[lv][i].x)&&balls[t].y ==sh[lv][i].y )) {

				balls[t].ystatus = (balls[t].ystatus + 1) % 2;
				sounds(6);
			}
			else if ((lv >= 1 && balls[t].ystatus == 1) && ((balls[t].x == sh[lv][i].x) && balls[t].y == sh[lv][i].y)) {

				balls[t].ystatus = (balls[t].ystatus + 1) % 2;
				sounds(6);
			}
		}

		consoleBuffer[balls[t].x + screen_x * balls[t].y].Char.AsciiChar = 'O';
		consoleBuffer[balls[t].x + screen_x * balls[t].y].Attributes =r;
	}


}
void bullet_move() {
	for (int j = 0; j < 5; j++)
	{
		if (bullets[j].status == 1 && lv==3)
		{
			
			for (int i = 0; i < 300 ; i++)
			{
				if (bullets[j].x == blocks[i].x ) {
					blocks[i].hp--;
					score++;
				}
				if (bullets[j].y == devils.y && bullets[j].x == devils.x)
				{
					score += 50;
					sounds(5);
					score++;
					devils.hp--;
				}
				consoleBuffer[bullets[j].x + screen_x * bullets[j].y].Char.AsciiChar = '|';
				consoleBuffer[bullets[j].x + screen_x * bullets[j].y].Attributes = 79;
				if (bullets[j].y > 3)
				{
					bullets[j].y--;
				}
				else bullets[j].status = 0;

			}
			for (int i = 0; i < (delay / 100); i++)
			{
				if (bullets[j].x == stars[i].x ) {
						score += 10;
						stars[i].x = rand() % screen_x;
						stars[i].y = 3 + rand() % 20;
						sounds(3);
				}
			}
			std::thread bomb3(Beep, 650, 40);
			bomb3.detach();
		}
		else if (bullets[j].status == 1 && lv == 2) {
			if (bullets[j].y == devils.y && bullets[j].x == devils.x)
			{
				score += 50;
				sounds(5);
				score++;
				devils.hp--;
			}
			for (int i = 0; i < 300; i++)
			{
				
				if (bullets[j].x == blocks[i].x && bullets[j].y == blocks[i].y) {
					blocks[i].hp--;
					score++;
					sounds(5);
				}
				if (bullets[j].y > 3)
				{	
				}
				else bullets[j].status = 0;
			}
			for (int i = 0; i < (delay / 100); i++)
			{
				if (bullets[j].x == stars[i].x) {
					score += 10;
					stars[i].x = rand() % screen_x;
					stars[i].y = 3 + rand() % 20;
					sounds(3);
				}
			}
			consoleBuffer[bullets[j].x + screen_x * bullets[j].y].Char.AsciiChar = '|';
			consoleBuffer[bullets[j].x + screen_x * bullets[j].y].Attributes = 79;
			bullets[j].y--;
		}
	}
}
void sounds(int song) {
	if (song == 0)
	{
		std::thread brick(Beep, 200, 50);
		brick.detach();
	}
	else if (song == 1) 
	{
		std::thread g(Beep, 700, 60);
		g.detach();
		std::thread h(Beep, 800, 60);
		h.detach();
		std::thread i(Beep, 900, 60);
		i.detach();
		std::thread j(Beep, 100, 60);
		j.detach();
	}
	else if (song ==2)
	{
		std::thread g01(Beep, 1500 - times * 75, 60);
		g01.detach();
		std::thread g02(Beep, 900 - times * 75, 60);
		g02.detach();
		std::thread g03(Beep, 1200 - times * 75, 60);
		g03.detach();
	}
	else if (song ==3)
	{
		std::thread bomb2(Beep, 1950, 120);
		bomb2.detach();
	}
	else if (song == 4) {
		std::thread g02(Beep, 1500 ,220);
		g02.detach();
		std::thread g03(Beep, 1200 , 160);
		g03.detach();
		std::thread g04(Beep, 200, 160);
		g04.detach();
	}
	else if (song == 5) {
		std::thread g06(Beep, 1500, 120);
		g06.detach();
	}
	else if (song == 6) {
		std::thread g07(Beep, 2000, 120);
		g07.detach();
	}
	else {
		std::thread brick2(Beep, 400, 50);
		brick2.detach();
	}
}
void summonAD() {
	if (timesx%1101 == 1100&& angels.status==0 && devils.status==0)
	{
		random = 1 + (rand() % 8);
		randomcharm = 1 + (rand() % 2);
		if (randomcharm == 1)
		{
			angels.y = 5 + (rand() % 30);
			angels.status = 1 + (rand() % 2);
			if (random>6)
			{
				if (lv<3)
				{
					lv++;
				}
				if (balllv<3)
				{
					balllv++;
				}
				
				
			}
			else
			{
				score += 1000;
			}
			ballcache = balllv;
			lvcache = lv;
			timescache = times;
			delaycache = delay;
			times = 5;
		}
		else {
			devils.y = 5 + (rand() % 30);
			devils.status = 1 + (rand() % 2);
			ballcache = balllv;
			lvcache = lv;
			delaycache = delay;
			timescache = times;
		}
		if (angels.status ==1)
		{
			angels.x = 2;
		}
		else if (angels.status == 2)
		{
			angels.x = 75;
		}
		
		if (devils.status==1)
		{
			devils.x =2;
			devils.hp = (timesx / 1000) + 1 + rand() % (5);
		}
		else {
			devils.x = 76;
			devils.hp = (timesx / 1000) + 1 + rand() % (5);
		}
		

	}
	if (randomcharm == 1)
	{

		if (angels.status == 1)
		{
			if (angels.x >= 0 && angels.x < screen_x)
			{
				consoleBuffer[angels.x + screen_x * (angels.y + 1)].Char.AsciiChar = 153;
				consoleBuffer[angels.x + screen_x * (angels.y + 1)].Attributes = 207;
				consoleBuffer[angels.x + screen_x * (angels.y + 2)].Char.AsciiChar = 208;
				consoleBuffer[angels.x + screen_x * (angels.y + 2)].Attributes = 207;
			}
			if (angels.x - 1 >= 0 && angels.x - 1 < screen_x)
			{
				if (angels.status == 1 && angels.x % 2 == 1)
				{
					consoleBuffer[angels.x - 1 + screen_x * (angels.y + 1)].Char.AsciiChar = 184;
					consoleBuffer[angels.x - 1 + screen_x * (angels.y + 1)].Attributes = 16 + rand() % 200;
				}
				else {
					consoleBuffer[angels.x - 1 + screen_x * (angels.y + 1)].Char.AsciiChar = 170;
					consoleBuffer[angels.x - 1 + screen_x * (angels.y + 1)].Attributes = 79;
				}
			}
			else if (angels.x - 1 == screen_x)
			{
				{
					angels.status = 0;
					random = 0;
					randomcharm = 0;
					balllv = ballcache;
					lv = lvcache;
					delay = delaycache;
				}
			} 

			if (timesx % 3 == 0)
			{
				angels.x++;
				if (angels.x % 6 < 3)
				{
					angels.y--;
				}
				else angels.y++;
			}
			
		}
		if (angels.status == 2)
		{
			if (angels.x >= 0 && angels.x < screen_x)
			{
				consoleBuffer[angels.x + screen_x * (angels.y + 1)].Char.AsciiChar = 153;
				consoleBuffer[angels.x + screen_x * (angels.y + 1)].Attributes = 207;
				consoleBuffer[angels.x + screen_x * (angels.y + 2)].Char.AsciiChar = 208;
				consoleBuffer[angels.x + screen_x * (angels.y + 2)].Attributes = 207;
			}
			if (angels.x + 1 >= 0 && angels.x < screen_x)
			{
				if (angels.x % 2 == 1)
				{
					consoleBuffer[angels.x + 1 + screen_x * (angels.y + 1)].Char.AsciiChar = 184;
					consoleBuffer[angels.x + 1 + screen_x * (angels.y + 1)].Attributes = rand() % 200;
				}
				else {
					consoleBuffer[angels.x + 1 + screen_x * (angels.y + 1)].Char.AsciiChar = 170;
					consoleBuffer[angels.x + 1 + screen_x * (angels.y + 1)].Attributes = 79;
				}
				if (timesx % 3 == 0)
				{
					angels.x--;
					if (angels.x % 6 < 3)
					{
						angels.y--;
					}
					else angels.y++;

				}
			}
			else {
				angels.status = 0;
				random = 0;
				randomcharm = 0;
				balllv = ballcache;
				lv = lvcache;
				delay = delaycache;
			}

		}
	}

	if (randomcharm == 2)
	{
		if (devils.status ==1 )
		{
			if (devils.x >= 1 && devils.x < screen_x-1)
			{
				
				consoleBuffer[devils.x  + screen_x * (devils.y + 1)].Char.AsciiChar = 237;
				consoleBuffer[devils.x  + screen_x * (devils.y + 1)].Attributes = 79;
			}
			if (devils.x - 1 >= 0 && devils.x - 1 < screen_x)
			{
				if (devils.x % 2 == 1)
				{
					consoleBuffer[devils.x - 1 + screen_x * (devils.y + 1)].Char.AsciiChar = 184;
					consoleBuffer[devils.x - 1 + screen_x * (devils.y + 1)].Attributes = 79;
				}
				else {
					consoleBuffer[devils.x - 1 + screen_x * (devils.y + 1)].Char.AsciiChar = 170;
					consoleBuffer[devils.x - 1 + screen_x * (devils.y + 1)].Attributes = 79;
				}
			}
			else {
				devils.status = 0;
				random = 0;
				randomcharm = 0;
				balllv =ballcache ;
				lv=lvcache ;
				delay = delaycache ;
				times = timescache  ;
			}
			if (timesx % 5 == 0)
			{
				devils.x++;
				if (devils.x % 8 < 4)
				{
					if (rand() % 2 == 1 && devils.y < screen_y-10)
						devils.y++;
					else if (devils.y > 0) devils.y--;
				}
				else
					if (devils.y < screen_y-10 && rand() % 2 == 1) devils.y++;
					else devils.y--;
				
			}
			if (rand() % 8 == 0)
			{
				if (score>10)
				{
					score-=10;
				}

			}
		}
		if (devils.status == 2)
		{
			if (devils.x+1 >= 1 && devils.x <= screen_x)
			{

				consoleBuffer[devils.x + screen_x * (devils.y + 1)].Char.AsciiChar = 237;
				consoleBuffer[devils.x + screen_x * (devils.y + 1)].Attributes = 79;
			}
			if (devils.x +1 >= 0 && devils.x  <= screen_x)
			{
				if ( devils.x % 2 == 1)
				{
					consoleBuffer[devils.x + 1 + screen_x * (devils.y + 1)].Char.AsciiChar = 184;
					consoleBuffer[devils.x + 1 + screen_x * (devils.y + 1)].Attributes = 79;
				}
				else {
					consoleBuffer[devils.x + 1 + screen_x * (devils.y + 1)].Char.AsciiChar = 170;
					consoleBuffer[devils.x + 1 + screen_x * (devils.y + 1)].Attributes = 79;
				}
			}
			else {
				devils.status = 0;
				random = 0;
				randomcharm = 0;
				balllv = ballcache;
				lv = lvcache;
				delay = delaycache;
				times = timescache;
			}
			if (timesx % 6 == 0)
			{
				devils.x--;
				if (devils.x % 8 < 4)
				{
					if (rand() % 2 == 1 && devils.y < screen_y - 10)
						devils.y++;
					else if (devils.y > 0) devils.y--;
				}
				else
					if (devils.y < screen_y - 10 && rand() % 2 == 1) devils.y++;
					else devils.y--;
			}
			if (rand() % 10 == 0)
			{
				
			}
		}
		if (devils.hp <= 0)
		{
			devils.status = 0;
			random = 0;
			randomcharm = 0;
			balllv = ballcache;
			lv = lvcache;
			delay = delaycache;
			times = timescache;
			score += 500 + timesx / 2;
			sounds(2);
		}
	}
	
}
void itemdrop(int angelstatus,int devilstatus) {

	if (angelstatus > 0 && random >0) {
		if (random <3)
		{
			times = 0;
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = '+';
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
		}
		else if (random <6)
		{
			if (delay >10)
			{
				delay -= 5;
				consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = '<';
				consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
			}
		}
		else  if (random>6)
		{
			times = 0;
			balllv = 3;
			delay = 0;
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 197;
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;

		}

	}
	if (devilstatus >0 && random> 0)
	{
		
		if (random < 3)
		{
			balllv = 2;
			if (lv < 2) lv = 2;
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = '!';
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
		}
		else if (random < 6)
		{
			if (godbullets.status == 0)
			{
				godbullets.x = devils.x;
				godbullets.y = devils.y;
				godbullets.status = 1;
			}
			if (lv < 2) lv = 2;
			if (delay < 3000)
			{
				delay += 15;
				consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar = '>';
				consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;
			}
		}
		else  if (random > 6)
		{
			if (godbullets.status == 0)
			{
				godbullets.x = devils.x;
				godbullets.y = devils.y;
				godbullets.status = 1;
			}
			lv = 2;
			balllv = 1;
			if (score>15)
			{
				score -= 10;
			}
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Char.AsciiChar ='X';
			consoleBuffer[55 - strlen(name) - 4 - 7 + times / 2 - timess + screen_x * 0].Attributes = 18;

		}
		consoleBuffer[47 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 'H';
		consoleBuffer[48 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Char.AsciiChar =  'P';
		consoleBuffer[49 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 48 +devils.hp;
		consoleBuffer[47 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Attributes = 18;
		consoleBuffer[48 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Attributes = 18;
		consoleBuffer[49 - strlen(name) - 4 - 5 + times / 2 - timess + screen_x * 0].Attributes = 67;

	}
}
void padhelp() {
	if (padhelper[4] < screen_x-1 && padhelperstatus == 1)
	{
			padhelper[0]= padhelper[0]+1;
			padhelper[1] = padhelper[1] + 1;
			padhelper[2] = padhelper[2] + 1;
			padhelper[3] = padhelper[3] + 1;
			padhelper[4] = padhelper[4] + 1;
			consoleBuffer[padhelper[0] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';
			consoleBuffer[padhelper[0] + screen_x * (screen_y - 3)].Attributes = 4;
			consoleBuffer[padhelper[1] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';
			consoleBuffer[padhelper[1] + screen_x * (screen_y - 3)].Attributes = 4;
			consoleBuffer[padhelper[2] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';

			consoleBuffer[padhelper[2] + screen_x * (screen_y - 3)].Attributes = 4;
			consoleBuffer[padhelper[3] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';
			consoleBuffer[padhelper[3] + screen_x * (screen_y - 3)].Attributes = 4;
			consoleBuffer[padhelper[4] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';
			consoleBuffer[padhelper[4] + screen_x * (screen_y - 3)].Attributes = 4;
		if (padhelper[4] >= screen_x-1)
		{
			padhelperstatus = 0;
			padhelper[0] = screen_x-5;
			padhelper[1] = screen_x - 4;
			padhelper[2] = screen_x - 3;
			padhelper[3] = screen_x - 2;
			padhelper[4] = screen_x-1;
		}
	}
	else if (padhelper[0]> 0 && padhelperstatus ==0)
	{
		for (int j = 0; j < 5; j++)
		{
			padhelper[j]= padhelper[j]-1;
			consoleBuffer[padhelper[j] + screen_x * (screen_y - 3)].Char.AsciiChar = '-';
			consoleBuffer[padhelper[j] + screen_x * (screen_y - 3)].Attributes = 4;
		}
		if (padhelper[0] <= 0)
		{
			padhelperstatus = 1;
		}
	}

}
void bullmove() {
	if (godbullets.status==1)
	{
		if (godbullets.y > 1 && godbullets.y < screen_y-2)
		{
			godbullets.y++;
		}
		else godbullets.status = 0;
		if (godbullets.x<posx)
		{
			godbullets.x++;
		}
		else if (godbullets.x > posx) godbullets.x--;
		consoleBuffer[godbullets.x + screen_x * godbullets.y].Char.AsciiChar = 207;
		consoleBuffer[godbullets.x + screen_x * godbullets.y].Attributes = 4;
		if (godbullets.x ==posx && godbullets.y == screen_y - 3 + position)
		{
			times++;
			sounds(3);
		}
	}




}
void adjustpow(int &adjusttime) {
	if (timesx -adjusttime>500&& !devils.status && !angels.status )
	{
		adjusttime = timesx;
		
		if (power >2) power--;
		else if (lv > 2) { lv--; }
		else if (balllv > 1) balllv--;

		else if (ballcount > 1) ballcount--;

	}


}
int  ultimatemove[6] = { 0,0,0,0,0,0 }, wavecount = 0, wavecolor = 7;
void ultimate_power_set() {
	for (int r = 0; r <=wavecount; r++)
	{
		wavecolor = 1 + rand() % 15;
		ultimatemove[r]++;
		for (int i = 0; i < 80; i++)
		{
			if (i < 40)
			{
				ultimate[i][r].x = i;
				ultimate[i][r].y = screen_y - 1 - (i / 4) - ultimatemove[r];
				if (ultimate[i][r].y > 1 && ultimate[i][r].status == 1)
				{
					consoleBuffer[ultimate[i][r].x + screen_x * ultimate[i][r].y].Char.AsciiChar = 207;
					consoleBuffer[ultimate[i][r].x + screen_x * ultimate[i][r].y].Attributes = wavecolor;
					ultimate[i][r].status = 1;
				}
				else ultimate[i][r].status = 0;
				
			}
			if (i > 39) {
				ultimate[i][r].x = i;
				ultimate[i][r].y = screen_y - 14 + ((i - 24) / 4) - ultimatemove[r];
				if (ultimate[i][r].y > 1 && ultimate[i][r].status == 1)
				{
					consoleBuffer[ultimate[i][r].x + screen_x * ultimate[i][r].y].Char.AsciiChar = 207;
					consoleBuffer[ultimate[i][r].x + screen_x * ultimate[i][r].y].Attributes = wavecolor;
					ultimate[i][r].status = 1;
				}
				else ultimate[i][r].status = 0;
			}
		}
		

	}
	if (wavecount < 5&&ultimatemove[wavecount]>4)
		wavecount++;
	else if(wavecount ==5 && ultimatemove[4]>=79) 
		wavecount = 0;


}
void ultimate_reset() {
	{
		for (int r = 0; r <6; r++)
		{
			for (int i = 0; i < 80; i++)
			{
				ultimate[i][r].status = 1;

			}
			ultimatemove[r] = 0;
		}
		wavecount = 0;
	}

}
void ultimate_burst() 
{
	for (int r = 0; r <5; r++)
	{
		for (int i = 0; i < 80; i++)
		{
			if (ultimate[i][r].status==1)
			{
				for (int k = 0; k < (delay / 100); k++)
				{
						if (ultimate[i][r].y == stars[k].y) {
							score += 1;
							stars[k].x = rand() % screen_x;
							stars[k].y = 3 + rand() % 10;
						}


				}
				for (int b = 0; b <300; b++)
				{
					if(ultimate[i][r].y ==blocks[b].y)
						if (ultimate[i][r].x == blocks[b].x&& blocks[b].hp<3)
						{
							score += blocks[b].hp;
							blocks[b].hp = 0;
						}
				}
				if(ultimate[i][r].y==1 && devils.y==1)
					if (ultimate[i][r].x== devils.x)
					{
						if (ultimate[i][r].y == devils.y) {
							if (devils.hp > 0)
								devils.hp -= 1;
						}

					}
			}
		}
	}


}

void mainmenu() {
	int count=0;
	char pressa[] = { 'P','r','e','s','s', ' ','\'','A','\'', 'F', 'o', 'r',' ', 'T','O','P',' ', '5'};
	char pressSpace[] = { 'P','r','e','s','s',' ','S','p','a','c','e',' ', 't','o',' ', 'P','l','a','y','!','!' };
	int first[] = { 223,' ', ' ',' ',' ', 223, ' ',223,223,223,223,' ',' ',223,223,223,223,223, ' ',223,223,223,223,' ',' ',' ',223,223,223,223,223,223,' ',223,223,223,223,223,' ',' ', ' ' ,' ',223,223,223,223,223, ' ',' ', ' ',223,223,223,223, ' ', ' ',223,223,223,223,223,' '};
	int second[] = { 223,223, ' ',' ',223,223,' ',223, ' ',' ',' ',' ',' ', ' ',' ', 219 ,' ',' ',' ',223, ' ',' ', ' ' ,' ', ' ', ' ',223,' ',' ',' ',' ',223,' ',223,' ',' ',' ',' ',223 ,' ', ' ' ,' ',223,' ',' ',' ',' ',223,' ',223,' ',' ',' ',' ',223,' ',223,' ',' ',' ',' ',223, };
	int third[] = { 223, ' ', 223,223,' ', 223,' ', 223, 223,223,223,223,' ', ' ',' ', 219 ,' ',' ',' ' , 223, 223,223,223,223, ' ',' ',223,' ',' ',' ',' ',223,' ',223,223,223,223,223,' ',' ', ' ' ,' ',223,223,223,223,223,' ', ' ',223,223,223,223,223,223,' ',223,' ',' ',' ',' ',223, };
	int fourth[] = { 223,' ',' ',' ',' ',223, ' ', 223,' ', ' ',' ', ' ' ,' ', ' ',' ', 219 ,' ',' ',' ',223, ' ',' ', ' ' ,' ', ' ',' ',223,' ',' ',' ',' ',223,' ',223,' ',' ',' ',223,' ',' ', ' ' ,' ',223,' ',' ',' ',' ', ' ',' ',223,' ',' ',' ',' ',223,' ',223,' ',' ',' ',' ',223, };
	int fifth[] = { 223,' ',' ',' ',' ',223, ' ', 223,223,223,223,223,223, ' ',' ', 219 ,' ',' ',' ' , 223,223,223,223,223,223, ' ',223,223,223,223,223,223,' ',223,' ',' ',' ',' ',' ',148,' ', ' ' ,223,' ',' ',' ',' ', ' ',' ',223,' ',' ',' ',' ',223, ' ',223,223,223,223,223,' ' };
	int pa[] = {223,223 ,223,223,' ',223,223 ,223,223,' ',219,' ',219 };
	int la[] = { 223,' ',220,220,' ',223,' ',' ',223,' ',219 };
	int aa[] = {223,' ',' ',220,' ' ,223,' ',' ',223,' ',' ' };
	int ya[] = {223,223 ,223 ,223,' ',223,223 ,223,223,' ',223 };
	int to[] = {223,223,223,223,223,' ', 223,223,223,223,' ',223,223,223,' ' ,' ',' ',' ',' ',223,223,223,223 ,' ',219 };
	int oo[] = {' ',' ',223,' ',' ',' ', 223 , ' ',' ', 223,' ', 219 ,' ',' ',223,' ',' ',' ',' ',223, ' ',' ',' ',' ',219};
	int po[] = { ' ',' ',223,' ',' ' ,' ', 223, ' ',' ', 223,' ',219,223,223,223,' ',' ',' ',' ',223,223, 223,223, ' ', 219};
	int fo[] = { ' ',' ',223,' ',' ' ,' ', 223,223,223,223,' ', 219 ,' ',' ',' ',' ',' ',' ',' ',220,220,220,223,' ',220 };

	int ex[] = { 223,223,223,223,223,' ',223,' ',' ',' ',223,' ',' ',223,' ',223,223,223,223,223,223,223,' ',223 };
	int xx[] = { 223,' ', ' ', ' ', ' ',' ',223,' ',' ',' ',223,' ',' ',' ',' ',' ',' ',' ',223,' ',' ',' ' ,219 };
	int ix[] = {223,223,223,223,223 ,' ',' ',' ',223,' ',' ',' ',' ',223,' ',' ',' ',' ',223,' ',' ',' ',219 };
	int tx[] = { 223,' ', ' ', ' ', ' ',' ',' ',223,' ',223,' ',' ',' ',223,' ' ,' ',' ',' ',223,' ',' ',' ',223 };
	int sx[] = { 223,223,223,223,223 ,' ',223,' ',' ',' ',223,' ',' ',223,' ',' ',' ',' ',223,' ',' ',' ',237 };
	int start = 9, playposition = 10, step = 0, exitposition = 30;;
	int meteorColor = 16;
	setMode();
	while (sub) {
		do {
			count++;
			padhelp();
			clearall_buffer();
			ultimate_power_set();
			inputUpdate();
			step = (step + 1) % 144;
			if (step % 2 == 1) meteorColor = (1 + (rand() % 15));
			for (int i = 0; i < 62; i++)
			{
				
				if (step < 60 && step != 20 || step>140) {
					consoleBuffer[start + i + screen_x * 10].Char.AsciiChar = first[i];
					consoleBuffer[start + i + screen_x * 10].Attributes = meteorColor;
				}
				if (step < 70 && step != 20 || step>135) {
					consoleBuffer[start + i + screen_x * 11].Char.AsciiChar = second[i];
					consoleBuffer[start + i + screen_x * 11].Attributes = meteorColor;
				
				}
				if (step < 80 && step != 20 || step>130) {
					consoleBuffer[start + i + screen_x * 12].Char.AsciiChar = third[i];
					consoleBuffer[start + i + screen_x * 12].Attributes = meteorColor;
				
				}
				if (step < 90 && step != 20 || step>125) {
					consoleBuffer[start + i + screen_x * 13].Char.AsciiChar = fourth[i];
					consoleBuffer[start + i + screen_x * 13].Attributes = meteorColor;
				}
				
				if(step <100 && step != 20|| step>120){
					consoleBuffer[start + i + screen_x * 14].Char.AsciiChar = fifth[i];
					consoleBuffer[start + i + screen_x * 14].Attributes = meteorColor;
				}
				
			}
			for (int i = 0; i < 21; i++)
			{
				consoleBuffer[50 + i + screen_x * 20].Char.AsciiChar = pressSpace[i];
				consoleBuffer[50 + i + screen_x * 20].Attributes = wavecolor;
				if (i < 18) {
					consoleBuffer[50 + i + screen_x * 25].Char.AsciiChar = pressa[i];
					consoleBuffer[50 + i + screen_x * 25].Attributes = wavecolor;
				}
			}

			if (goStatus) {
				int t = 16 + ((rand() % 7) * 16);
				for (int i = 0; i < 6; i++)
				{
					for (int j = 0; j < 13; j++)
					{
						consoleBuffer[9+j +screen_x * (19+i)].Char.AsciiChar = 254;
						consoleBuffer[9+j + screen_x * (19+i)].Attributes =t;
					}
				}
			}
			if (topStatus) {
				int t = 16 + ((rand() % 7) * 16);
				for (int i = 0; i < 6; i++)
				{
					for (int j = 0; j < 27; j++)
					{
						consoleBuffer[27 + j + screen_x * (29 + i)].Char.AsciiChar = 254;
						consoleBuffer[27 + j + screen_x * (29 + i)].Attributes = t;
					}
				}
			}

			if (exitStatus) {
				int t = 16 + ((rand() % 7) * 16);
				for (int i = 0; i < 7; i++)
				{
					for (int j = 0; j < 27; j++)
					{
						consoleBuffer[exitposition+16 + j + screen_x * (41 + i)].Char.AsciiChar = 254;
						consoleBuffer[exitposition+16 + j + screen_x * (41 + i)].Attributes = t;
					}
				}
			}
			for (int i = 0; i < 11; i++)
			{
				consoleBuffer[playposition + i + screen_x * 20].Char.AsciiChar = pa[i];
				consoleBuffer[playposition + i + screen_x * 20].Attributes = gocolor;
				consoleBuffer[playposition + i + screen_x * 21].Char.AsciiChar = la[i];
				consoleBuffer[playposition + i + screen_x * 21].Attributes = gocolor;
				consoleBuffer[playposition + i + screen_x * 22].Char.AsciiChar = aa[i];
				consoleBuffer[playposition + i + screen_x * 22].Attributes = gocolor;
				consoleBuffer[playposition + i + screen_x * 23].Char.AsciiChar = ya[i];
				consoleBuffer[playposition + i + screen_x * 23].Attributes = gocolor;
			}
			for (int i = 0; i < 25; i++)
			{
				consoleBuffer[playposition+18 + i + screen_x * 30].Char.AsciiChar = to[i];
				consoleBuffer[playposition+18 + i + screen_x * 30].Attributes = gocolor;
				consoleBuffer[playposition + 18 + i + screen_x * 31].Char.AsciiChar = oo[i];
				consoleBuffer[playposition + 18 + i + screen_x * 31].Attributes = gocolor;
				consoleBuffer[playposition + 18 + i + screen_x * 32].Char.AsciiChar = po[i];
				consoleBuffer[playposition + 18 + i + screen_x * 32].Attributes = gocolor;
				consoleBuffer[playposition + 18 + i + screen_x * 33].Char.AsciiChar = fo[i];
				consoleBuffer[playposition + 18 + i + screen_x * 33].Attributes = gocolor;
			}
			for (int i = 0; i < 23; i++)
			{
				consoleBuffer[exitposition + 18 + i + screen_x * 42].Char.AsciiChar = ex[i];
				consoleBuffer[exitposition + 18 + i + screen_x * 42].Attributes = gocolor;
				consoleBuffer[exitposition + 18 + i + screen_x * 43].Char.AsciiChar = xx[i];
				consoleBuffer[exitposition + 18 + i + screen_x * 43].Attributes = gocolor;
				consoleBuffer[exitposition + 18 + i + screen_x * 44].Char.AsciiChar = ix[i];
				consoleBuffer[exitposition + 18 + i + screen_x * 44].Attributes = gocolor;
				consoleBuffer[exitposition + 18 + i + screen_x * 45].Char.AsciiChar = tx[i];
				consoleBuffer[exitposition + 18 + i + screen_x * 45].Attributes = gocolor;
				consoleBuffer[exitposition + 18 + i + screen_x * 46].Char.AsciiChar = sx[i];
				consoleBuffer[exitposition + 18 + i + screen_x * 46].Attributes = gocolor;
			}
			consoleBuffer[posx + screen_x * posy].Char.AsciiChar = 'O';
			consoleBuffer[posx + screen_x * posy].Attributes = wavecolor;
			fill_star();
			fill_buffer_to_console();
			Sleep(50);
			if (count % 250 == 249)
				ultimate_reset();
			ultimate_burst();
			if (delay <= 4000)
			{
				delay += 1;
			}
			setMode();
		} while (sub);
		reset_blocks();
		score = 0;
		timesx = 0;
		times = 0;
		lv = 2;
		ballcount = 1;
		balllv = 1;
		power = 0;
		displaycolorful = 0;
		angels.status = 0;
		devils.status = 0;
		ultimate_reset();
		defendersetup = 0;
		random = 0;
		delay = 0;
	}
}

void display_percent() {

	if(timesx - charge >= 750) displaycolorful = ((rand() % 7) * 16);
	consoleBuffer[41 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 'X';

	consoleBuffer[41 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 18+ displaycolorful;

	if (timesx - charge >= 150) {
		consoleBuffer[42 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 175;
		consoleBuffer[42 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 21 + displaycolorful;
	}
	if (timesx - charge >= 300) {
		consoleBuffer[43 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 175;
		consoleBuffer[43 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 23 + displaycolorful;
	}
	if (timesx - charge >= 450) {
		consoleBuffer[44 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 175;
		consoleBuffer[44 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 18 + displaycolorful;
	}
	if (timesx - charge >= 600) {
		consoleBuffer[45 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 175;
		consoleBuffer[45 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 18 + displaycolorful;
	}
	if (timesx - charge >= 750) {
		consoleBuffer[46 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Char.AsciiChar = 175;
		consoleBuffer[46 - strlen(name) - 4 - 6 + times / 2 - timess + screen_x * 0].Attributes = 18 + displaycolorful;
	}
}

