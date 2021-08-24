#pragma once
#include <sys/time.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define Sleep(x) usleep(x*1000)

int _kbhit(){
	int r;
	fd_set rfds;
	struct timeval tv = {0};
	struct termios term, oterm;
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	cfmakeraw(&term);
	tcsetattr(0, TCSANOW, &term);
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	r = select(1, &rfds, NULL, NULL, &tv);
	tcsetattr(0, TCSANOW, &oterm);
	return r;
}