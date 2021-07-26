#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "vm.h"

int main(int argc, char **argv)
{
		if(argc < 4) {
				printf("usage: ./input_gen [# PAGE] [# PROC] [# request] \n");
				return -1;
		}

		int NUM_PAGE = atoi(argv[1]);
		int NUM_PROC = atoi(argv[2]);
		int req_num = atoi(argv[3]);
		int i, pid, page, offset, addr;
		char type, byte;

		srand(time(NULL));

		for(i = 0; i < req_num; i++)
		{
				pid = rand() % NUM_PROC;
				page = rand() % NUM_PAGE;
				offset = rand() % 0xFF;
				if(rand()%2 == 0) type = 'R';
				else type = 'W';
				addr = (page << 8) + offset;
		
				if(type == 'R') printf("%d %c 0x%04x\n", pid,type, addr);
				else {
						if(rand() % 2 == 0) {
								byte = rand() % 26 + 65;
						} else {
								byte = rand() % 26 + 97;
						}
						printf("%d %c 0x%04x %c\n", pid,type, addr, byte);
				}
		}

		return 0;
}

