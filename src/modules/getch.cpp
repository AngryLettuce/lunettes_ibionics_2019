#include <unistd.h>
#ifdef __arm__
#include <termios.h>
#endif
#include <stdio.h>

#include "getch.h"
#ifdef __arm__
static struct termios old, current;
#endif
/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
    #ifdef __arm__
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
#endif
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
#ifdef __arm__
  tcsetattr(0, TCSANOW, &old);
#endif
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}


