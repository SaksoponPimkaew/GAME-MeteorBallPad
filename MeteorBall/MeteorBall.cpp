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
bool play = 1;

FILE* mptr;
int  defendersetup = 0, blocksetup=0;
char name[14] = "";
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	//setMode();
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	nameSetup();
	setMode();
	scoreSetup();
	setcursor(0);
	while (play)
	{

		inputUpdate();
		fill_star();
		ball_move();
		fill_defender_to_console();
		fill_blocks_to_console();
		//inputUpdate();
		displaybuff(timess, buff);
		displayname( timess);
		displayHP(timess);
		displayscore(score);
		fill_ship_to_buffer(posx, screen_y - 3 + position);
		bullet_move();
		fill_buffer_to_console();
		
		if (delay <= 3000)
		{
			delay += 1;
		}
		if (times>=10)
		{
			userinterface();
		}
		clearall_buffer();
		Sleep(55 - delay / 100);
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
	gotoxy(27, 27);
	printf("Enter Your Nickname: ");
	for (int i = 0; i < 12; i++)
	{
		char h = 8;
		gotoxy(48 + i, 27);
		scanf("%c", &name[i]);
		if (name[i] == ' ' || name[i] == '\r') {
			name[i] = '\0';
			gotoxy(24, 30);
			
			system("pause");
			break;
		}
		if (name[i] == h && i >= 0) {
			i -= 2;
			printf(" ");
			if (i == -2) i = -1;
		}
		else printf("%c", name[i]);





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
		for (int j = 24; j < 31; j++)
		{
			gotoxy(i, j);
			if (j==24 || j==30)
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
		lv = 1;
		times = 0;
		timesx = 0;
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
				if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
					userinterface();
				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
				{
					power = (power + 1) % 4;
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
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a') {
					ballcount = 1 + (ballcount) % 3;

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'z') {

					lv = (lv + 1) % 4;

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'r') {
					lv = 1;
					

				}
				else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'x') {

					if (delay < 3000)
					{
						delay = 3000;

					}
					else if (delay >= 3000)
					{
						delay = 0;
					}
				}
			}
			 if (eventBuffer[i].EventType == MOUSE_EVENT) {

				posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
				posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

				if (eventBuffer[i].Event.MouseEvent.dwButtonState &FROM_LEFT_1ST_BUTTON_PRESSED) {
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
	clearall_buffer();
	fill_buffer_to_console();
	gotoxy(38, 22);
	printf("TOP 5\n");
	mptr = fopen("data2.txt", "r");
	for (int i = 0; i < 5; i++)
	{
		fscanf(mptr, "%s %d %d\n", &p[i].name, &p[i].times, &p[i].score);
	}
	for (int i = 0; i < 5; i++)
	{
		gotoxy(32, 24+2*i);
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
	consoleBuffer[70 - timess + screen_x * 0].Attributes = 19+(lv*2)-1+((power)*8);

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
				k = rand() % 4;
				if (k==0)
				{
					times = 0;
				}
				else if (k==1)
				{
					if (lv<3)
					{
						lv++;
					}
					else score += 1500;
				}
				else if (k==2)
				{
					if (power<2)
					{
						power++;
					}
					else score += 1500;
				}
				else if (k == 3)
				{
					score += 1500;
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
	else {
		std::thread brick(Beep, 400, 50);
		brick.detach();
	}


}