#include<stdio.h>
#include<stdlib.h>
#include<curses.h>
#include<time.h>
#include<string.h>

void make_num(int lotto[])
{
	int i,j;
	srand(time(NULL));
	for(i=0;i<6;i++)
	{
		lotto[i] = rand()%45+1;

		for(j=0;j<i;j++)
		{
			if(lotto[i] == lotto[j])
			{
				i--;
				break;
			}
		}
	}
}

main()
{
	int i;
	int lotto[6];
	int col = 20;
	char str[3];
	make_num(lotto);
	initscr();
	clear();
	
	move(3,5);
	addstr("Waiting....");
	move(5,5);
	addstr("Computer extract 6 number... ");
	for(i=0;i<6;i++)
	{
		sprintf(str,"%d",lotto[i]);
		move(8,col);
		addstr(str);
		refresh();
		sleep(1);
		col +=5;
	}

	move(3,5);
	addstr("    Congratulation !!!  ");
	move(5,5);
	addstr("    You must win coming weekend game!!  ");
	move(30,50);
	getch();
	endwin();
}
