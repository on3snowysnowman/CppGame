
#include <curses.h>
#include <string>

using namespace std;

int main()
{	
    int ch;

	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    printw("Press any key");
    refresh();

    while(true){
        
        ch = getch();
        erase();
        printw("Your character is: ");
        printw("%i", ch);
        refresh();
    }
}