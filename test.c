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


  tests++;
	top = Pip_PageCount(part,area);
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
    page=allocPage();
    *(void**)page = (void*)0;

		for(i=0; i<top-1; i++){
			tmp = Pip_AllocPage();
			*(void**)tmp = page;
			page = tmp;
		}
    tests++;

		if(Pip_Prepare(part,area,(uint32_t)page,0x0)){
			log1("page prepared part=",1);
      atests++;
		}
		else{
		 log1("couldn't prep page, part=",0);
     Pip_Debug_PutHex((unsigned long)part);
     log1(", area=",1);
     Pip_Debug_PutHex((unsigned long)area);
     log1(", page=",1);
     Pip_Debug_PutHex((unsigned long)page);
     log1("\n",1);
   }
 	}
  tests++;
  if(Pip_PageCount(part,area)==0){
    log1("count on already prepared pages return 0 ",1);
    atests++;
  }
  else
    log1("count on already prepared pages dont return 0 ",0);
	log1("end of 'testOnPages(child)'",-1);
}

/* Test a complete area in addMem For the child
*	@param part the childpart
* @type uint32_t
* @return 0 if it succeed
* @type int
*/
int MultAddrTest(uint32_t part){
	log1("Multi addVaddr Test Start",-1);
	uint32_t childVadd,fathAdd; /* represent the virtual address of the child and the father address */
  log1("Trying to address ages used by kernel to child ",-1);
  tests++;
  atests++;
  for(fathAdd=0x0;fathAdd<0x700000;fathAdd+=0x1000){
    if(Pip_AddVAddr(fathAdd,part,0x710000,1,1,1)){
      log1("fathAdd ",-2);
      puthex(fathAdd); putc('\n');
      log1("Could move kernel space to child ",0);
      Pip_RemoveVAddr(part,0x710000);
      //atests--;
      //break;
    }
  }
  tests++;
  atests++;
  uint32_t * page = Pip_AllocPage();
	for(childVadd=0x300000;childVadd<0x800000;childVadd+=0x1000){
		if(Pip_AddVAddr((unsigned long)page,part,childVadd,1,1,1)){
			/*log1("addVAddr succeded ",-2);
			puthex(childVadd);putc('\n');*/
			Pip_RemoveVAddr(part,childVadd);
		}
		else{
      /*
      log1("addVaddr failed                ",-2);
			puthex(childVadd);
      putc('\n');*/
      //atests--;
      //break;
		}
	 }
   Pip_FreePage(page);

	log1("Multi addVaddr Test Ended ",-1);
	return 0;
}

void DumpValue(uint32_t add){
  puts("Hello :");
  puthex(*(uint32_t *)add);
  putc('\n');
}
