#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "disk.h"
#include "pagetable.h"
#include "list.h"

struct Node *head=NULL;

PT *pageTable;
Invert *frame_table;
STATS stats;

int i=0;
int is_page_hit(int pid, int pageNo, char type)
{
     if(type=='W')
       pageTable[pid].entry[pageNo].dirty=true; 
       // return the frame number if the page is available in the physical memory
       // otherwise return -1
       if(pageTable[pid].entry[pageNo].valid==true){
       return pageTable[pid].entry[pageNo].frameNo;

       }
		return -1;
}

int MMU(int pid, int addr, char type, bool *hit)
{
		int frameNo;
		int pageNo = (addr >> 8);
		int offset = addr - (pageNo << 8);
		int physicalAddr = 0;

		if(pageNo >= NUM_PAGE) {
				printf("invalid page number (NUM_PAGE = 0x%x): pid %d, addr %x\n", NUM_PAGE, pid, addr);
				return -1;
		}
		if(pid > (NUM_PROC-1)) {
				printf("invalid pid (valid pid = 0 - %d): pid %d, addr %x\n", NUM_PROC-1, pid, addr);
				return -1;
		}
		// hit
		frameNo = is_page_hit(pid, pageNo, type);

		//also need to update page table
		if(frameNo >= 0) {
 if(replacementPolicy==LRU ){
head=list_remove(head,frameNo);
head=list_insert_head(head,frameNo);
      }
				stats.hitCount++;
				physicalAddr = (frameNo << 8) + offset;

				*hit = true;
				return physicalAddr;
		}
		stats.missCount++;
		*hit = false;
		// miss. handle  pagefault
		if(freeFrame < NUM_FRAME){
            frameNo = freeFrame;
            freeFrame++;
            pageTable[pid].entry[pageNo].frameNo=frameNo;  //I do not have to decrease it? no processes will free?
head=list_insert_head(head,i++);
		} else{
frameNo= page_replacement( &head);
pageTable[pid].entry[pageNo].frameNo=frameNo;    if(pageTable[frame_table[frameNo].pid].entry[frame_table[frameNo].page].dirty==true)
swap_write(frameNo,frame_table[frameNo].pid,frame_table[frameNo].page);            pageTable[frame_table[frameNo].pid].entry[frame_table[frameNo].page].valid=false;
		}

		//update page table
        pageTable[pid].entry[pageNo].dirty=false;
        if(type=='W')pageTable[pid].entry[pageNo].dirty=true;
        pageTable[pid].entry[pageNo].valid=true;
        frame_table[frameNo].page=pageNo;
        frame_table[frameNo].pid=pid;
        swap_read(frameNo,pid,pageNo);

		physicalAddr = (frameNo << 8) + offset;
		return physicalAddr;
}

void pt_print_stats()
{
		int req = stats.hitCount + stats.missCount;
		int hit = stats.hitCount;
		int miss = stats.missCount;

		printf("Request: %d\n", req);
		printf("Page Hit: %d (%.2f%%)\n", hit, (float) hit*100 / (float)req);
		printf("Page Miss: %d (%.2f%%)\n", miss, (float)miss*100 / (float)req);

}

void pt_init()
{
		int i,j;
		pageTable = (PT*) malloc(sizeof(PT) * NUM_PROC);
		frame_table = (Invert*) malloc(sizeof(Invert*) * NUM_FRAME);
		stats.hitCount = 0;
		stats.missCount = 0;

		for(i = 0; i < NUM_PROC; i++) {
				pageTable[i].entry = (PTE*) malloc(sizeof(PTE) * NUM_PAGE);
				for(j = 0; j < NUM_PAGE; j++) {
						pageTable[i].entry[j].valid = false;
				}
		}
}

