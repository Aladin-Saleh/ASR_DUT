#include "ihm.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>

ihmNcu			ihm;

void infoLine(char *ip, char *port)
{
	wclear(ihm.infoLine);
	wrefresh(ihm.infoLine);
	wprintTime(ihm.infoLine, time(NULL));
	wattron(ihm.infoLine, COLOR_PAIR(1));
	wprintw(ihm.infoLine, "Connected to ");
	wattroff(ihm.infoLine, COLOR_PAIR(1));
	wattron(ihm.infoLine, COLOR_PAIR(3));
	wprintw(ihm.infoLine, "%s", ip);
	wattroff(ihm.infoLine, COLOR_PAIR(3));
	wattron(ihm.infoLine, COLOR_PAIR(1));
	wprintw(ihm.infoLine, ":");
	wattroff(ihm.infoLine, COLOR_PAIR(1));
	wattron(ihm.infoLine, COLOR_PAIR(4));// 3 avant
	wprintw(ihm.infoLine, "%s", port);
	wattroff(ihm.infoLine, COLOR_PAIR(3));
	wrefresh(ihm.infoLine);
}


void colors()
{
	start_color();
	use_default_colors();
	init_pair(1, -1, -1); // Default
	init_pair(2, COLOR_CYAN, -1);
	init_pair(3, COLOR_YELLOW, -1);
	init_pair(4, COLOR_RED, -1);
	init_pair(5, COLOR_BLUE, -1);
	init_pair(6, COLOR_MAGENTA, -1);
	init_pair(7, COLOR_GREEN, -1);
	init_pair(8, COLOR_WHITE, COLOR_RED);
}

void createChatBox()
{
	int	winx, winy;

	winx = COLS ;
	winy = LINES * 0.8;
	ihm.tchatWinBox = subwin(ihm.mainWin, winy, winx, 0, 0);
	box(ihm.tchatWinBox, 0, 0);
	mvwaddch(ihm.tchatWinBox, 0, (winx * 0.5) - 6, ACS_RTEE);
	wattron(ihm.tchatWinBox, COLOR_PAIR(3));
	mvwaddstr(ihm.tchatWinBox, 0, (winx * 0.5) - 5, " Messages ");
	wattroff(ihm.tchatWinBox, COLOR_PAIR(3));
	mvwaddch(ihm.tchatWinBox, 0, (winx * 0.5) + 5, ACS_LTEE);
	wrefresh(ihm.tchatWinBox);
	ihm.tchatWin = subwin(ihm.tchatWinBox, (winy - 2), winx - 2, 1, 1);
	scrollok(ihm.tchatWin, TRUE);
}

void createTextBox()
{
	ihm.textWinBox = subwin(ihm.mainWin, (LINES * 0.2) - 2, COLS, (LINES * 0.8)+1  , 0);
	ihm.textWin = derwin(ihm.textWinBox, (LINES * 0.2) - 4, COLS - 2, 1 , 1);


	box(ihm.textWinBox,0 , 0);
	wrefresh(ihm.textWinBox);

	scrollok(ihm.textWin, TRUE);
}

void createInfoLine()
{
	ihm.infoLine = derwin(ihm.mainWin, 1, COLS,  LINES - 1, 0);
}

void ihmStart()
{
	if ((ihm.mainWin = initscr()) == NULL)
		exit(1);
	//echo();
	//noecho();
	//nocbreak();
	cbreak();
	//	raw();
	//keypad(ihm.mainWin, TRUE);
	colors();
	createChatBox();
	createTextBox();
	createInfoLine();
}

void ihmEnd()
{
	delwin(ihm.tchatWin);
	delwin(ihm.tchatWinBox);
	delwin(ihm.usersWin);
	delwin(ihm.usersWinBox);
	delwin(ihm.textWin);
	delwin(ihm.textWinBox);
	delwin(ihm.infoLine);
	endwin();
}
void wprintTime(WINDOW *win, time_t ts)
{
	struct tm *timestamp;

	timestamp = localtime(&ts);

	wattron(win, COLOR_PAIR(1));
	wprintw(win, "[%02d", timestamp->tm_hour);
	wattroff(win, COLOR_PAIR(1));
	wattron(win, COLOR_PAIR(3));
	wprintw(win, ":");
	wattroff(win, COLOR_PAIR(3));
	wattron(win, COLOR_PAIR(1));
	wprintw(win, "%02d", timestamp->tm_min);
	wattroff(win, COLOR_PAIR(1));
	wattron(win, COLOR_PAIR(3));
	wprintw(win, ":");
	wattroff(win, COLOR_PAIR(3));
	wattron(win, COLOR_PAIR(1));
	wprintw(win, "%02d] ", timestamp->tm_sec);
	wattroff(win, COLOR_PAIR(1));
}

void ihmRefresh(){
	wrefresh(ihm.tchatWin);
	wcursyncup(ihm.textWin);
	wrefresh(ihm.textWinBox);
}

void readTextBox(char * user,char buf[], int size){
	int prefix;
	snprintf(buf,size,"[%s] ",user); 
	prefix = strlen(buf);
	wmove(ihm.textWin, 0, 0);
	wrefresh(ihm.textWin);
	if (size -prefix >=0) {
		getnstr(buf+prefix,size-prefix);
	}
	wclear(ihm.textWin);
	wrefresh(ihm.textWin);
	wprintTime(ihm.tchatWin, time(NULL));
	wprintw(ihm.tchatWin, "%s\n", buf);          
}