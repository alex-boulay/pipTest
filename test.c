#include <stdint.h>
#include <pip/fpinfo.h>
#include <pip/paging.h>
#include <pip/vidt.h>
#include <pip/api.h>
#include <pip/compat.h>
#include <pip/debug.h>
#include "main.h"
#include "test.h"


#define TDEBEUG 5

void log1(char* str,int tok)
{
    puts("|Test| ");
    puts(str);
    if(tok==0)
      puts( "--Failed\n");
    else if(tok==1)
      puts(" --Worked\n");
    else if(tok==-1){
      putc('\n');
    }
    return;
}

void parse_bootinfo(pip_fpinfo* bootinfo)
{
	if(bootinfo->magic == FPINFO_MAGIC)
    log1("\tBootinfo seems to be correct.\n",-1);
	else {
	   log1("\tBootinfo is invalid. Aborting.\n",-1);
	    PANIC();
	}

	log1("\tAvailable memory starts at ",-1);
	puthex((uint32_t)bootinfo->membegin);
	puts(" and ends at ");
	puthex((uint32_t)bootinfo->memend);
	puts("\n");

	log1("\tPip revision ",-1);
	puts(bootinfo->revision);
	puts("\n");

	return;
}

void testSet(){

}
/* Test for count prepare collect */
void testsSet2(){

}

/* Test the collect on pages
* @param part the child part
* @param area the area to test on
*/
int testOnPages(uint32_t part,uint32_t area){
	log1("test on page starting ",-1);
	uint32_t i,top,*tmp,*page;

	page=allocPage();
	*(void**)page = (void*)0;

  tests++;
	top =Pip_PageCount(part,area);
  if(top==3){
    log1("count tests on unalloccated page == 3",1);
    atests++;
  }
  else
    log1("count tests on unalloccated page !=3 ",0);
	log1("Page counting ",-2);
	putdec(top);
	puts(" on area : ");
	puthex(area);
	putc('\n');
	if(top>0){
		for(i=0; i<top-1; i++){
			tmp = Pip_AllocPage();
			*(void**)tmp = page;
			page = tmp;
		}
    tests++;
		if(Pip_Prepare(part,area,(uint32_t)page,0x0)){
			log1("page prepared ",1);
      atests++;
		}
		else
		 log1("couldn't prep page",0);
 	}
  if(Pip_PageCount(part,area)==0){
    log1("count on already prepared pages return 0 ",1);
  }
  else
    log1("count on already prepared pages dont return 0 ",0);
	log1("end of 'testOnPages(child)'",-1);
}

/* Test some virtual addresses
*	@param part the childpart
* @type uint32_t
* @return 0 if it succeed
* @type int
*/
int addVAddrTest(uint32_t part){
	log1("addVAddr Test Start",-1);
  /*
	uint32_t deathstar;
	for(deathstar=0x70C000;deathstar<=0x3F3D9000;deathstar+=0x1000){
		if(Pip_AddVAddr(deathstar,part,deathstar,1,1,1)){
			log1("addVAddr succeded ",-2);
			puthex(deathstar);putc('\n');
			Pip_RemoveVAddr(deathstar,part);
		}
		else{
			puthex(deathstar);
			log1(" addVaddr failed",-1);
		}
	}*/
	log1("addVaddr Test Ended ",-1);
	return 0;
}
