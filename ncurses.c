#include<ncurses.h>
#include<stdio.h>

main() 
{
int c;
unsigned int uint_screen_rows;
unsigned int uint_screen_cols;

WINDOW *win, *win2, *win21, *win1;

initscr();

getmaxyx(stdscr, uint_screen_rows, uint_screen_cols);
printw("lo schermo ha %d colonne e %d righe",uint_screen_cols, uint_screen_rows);
refresh();
      
win2 = newwin(uint_screen_rows/1.5, uint_screen_cols,0,0);box(win2,0,0);wrefresh(win2);
win  = newwin(uint_screen_rows/3, uint_screen_cols,uint_screen_rows/1.5,0);box(win,0,0);wrefresh(win);
win21 = newwin(uint_screen_rows/1.5-2, uint_screen_cols-2,1,1);wrefresh(win21);
win1  = newwin(uint_screen_rows/3-2, uint_screen_cols-2,uint_screen_rows/1.5+1,1);wrefresh(win1);
scrollok(win21, TRUE);
scrollok(win1, TRUE);
/*win2  = newwin( 20,20,5,5);;*/
wprintw(win2,"questo testo finisce nella finestra1");
wrefresh(win2);

/*win = newwin( 20,20,1,30);box(win,0,0);wrefresh(win);*/
wprintw(win,"questo testo finisce nella finestra2");
wrefresh(win);
     
cbreak();
noecho();

wprintw(win1,"hello word");
wrefresh(win1);

while ( (c = getch()) != '*' ) {

 if (c == '!') {
  clear();
 }
 wprintw(win1,"%c\n\r",c);
 wprintw(win21,"stringa\n");
 wrefresh(win1);
 wrefresh(win21);
}
 
refresh();
endwin();
}