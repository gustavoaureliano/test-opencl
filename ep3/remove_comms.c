/*
 * Este programa remove comentários em arquivos PGM
 * (texto entre # e '\n')
 */

#include <stdio.h>

enum Q {BEG_LINE, COMM_LINE, EOL_COMM, DATA};

int main()
{
  char c;
  enum Q state = BEG_LINE;

  while ((c = getchar()) != EOF) {
    switch (state) {
    case BEG_LINE:
      if (c == '#')
	state = COMM_LINE;
      else {
	putchar(c);
	state = DATA;
      }
      break;
    case DATA:
      if (c != '\n' && c != '#')
	putchar(c);
      else if (c == '#')
	state = EOL_COMM;
      else if (c == '\n') {
	putchar(c);
	state = BEG_LINE;
      }
      break;
    case EOL_COMM:
      if (c == '\n') {
	putchar('\n');
	state = BEG_LINE;
      }
      break;
    case COMM_LINE:
      if (c == '\n')
	state = BEG_LINE;
      break;
    }
  }
  
  return 0;
}
