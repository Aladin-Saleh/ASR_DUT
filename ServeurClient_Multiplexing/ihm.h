#ifndef _IHM_H 
#define _IHM_H

# include <ncurses.h>

typedef struct	ihmNcu
{
	WINDOW		*mainWin;
	WINDOW		*tchatWinBox;
	WINDOW		*tchatWin;
	WINDOW		*usersWinBox;
	WINDOW		*usersWin;
	WINDOW		*textWinBox;
	WINDOW		*textWin;
	WINDOW		*infoLine;
}				ihmNcu;

void			ihmStart();
void			ihmRefresh();
void			ihmEnd();

void			createChatBox();
void			createTextBox();
void			createInfoLine();

void			infoLine();
void			wprintTime();
void			readTextBox();
#endif
