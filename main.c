#include <stdint.h>
#include <pip/fpinfo.h>
#include <pip/paging.h>
#include <pip/vidt.h>
#include <pip/api.h>
#include <pip/compat.h>
#include <pip/debug.h>
#include "test.h"

#define BUFFSIZE 20
extern void* _fpart, *_efpart;
extern void* _spart, *_espart;

int atests=0;/* number of achieved tests */
int tests=0; /*number of tests */
int used;
int length,offset;
uint32_t BTVS[BUFFSIZE]; /* Buffer the vampire slayer : here to allocate memmory*/
volatile int currchild; /*tells if there is a child alive */

static const struct {uint32_t start, end;} fpart = {
	(uint32_t)&_fpart, (uint32_t)&_efpart,
};

void AllocAll(){
	used=0;
	for(;used<BUFFSIZE;used++){
		BTVS[used]=(uint32_t)Pip_AllocPage();
	}
	used=0;
}

INTERRUPT_HANDLER(timerAsm,timerHandler)
	log1("DEBUG TOKEN",-1);
	if(currchild!=0){
		log1("entry resume timer\n",-1);
		Pip_Resume(BTVS[0],1);
	}
  resume(0xFFFFFFFF,1);
}

/*
* The interruption triggering the Handler have to come from the child
* this interrupt allow me to test the deleting of Partitions
* atm i'm testing to delete one existing part, the same part but already deleted
* and a last part which never existed
*/
INTERRUPT_HANDLER(takebackAsm,takebackHandler)
	tests++;
	if(deletePartition(BTVS[0])){
		log1("Deleting Part : 0",1);
		currchild--;
		atests++;
	}
	else{
		log1("Couldn't delete part",0);
	}
	tests++;
	if(deletePartition(BTVS[0])){
		log1("Was abble to deleted again the child 0",0);
	}
	else{
		log1("Couldn't delete once again the child",1);
		atests++;
	}
	tests++;
	if(deletePartition(BTVS[5])){
			log1("Succeded to delete inexisting partition ",0);
	}
	else{
		log1("Infructuous attenpt to delete inexisting partition ",1);
		atests++;
	}
	resume(0xFFFFFFFF,1);
}

/*
* @define wrap multiple pages from an array
* @param tab the uint32_t array of allocated pages
* @type uint32_t array
* @param number the number of page to map from the array
* @type uint32_t (might change can't use 3Blions pages)
* @param name child the child partition to map on
* @type uint32_t
* @param place the place in the child memory to map on
* @type uint32_t
*/
int multiWrapp(uint32_t tab[],uint32_t number,uint32_t child,uint32_t place){
	int mwi,mwh;
	mwh=0;/* here if place is already used */
	for(mwi=0;mwi<number;mwi++){
		if(Pip_MapPageWrapper(tab[mwi],child,place+(mwh*0x1000))){
			puts("Failed to map on : ");
			puthex(place+(mwh*0X1000));
			putc('\n');
			mwi--;
		}
		else{
			puts("|Test| Mapping on : ");
			puthex(place+(mwh*0X1000));
			putc('\n');
		}
		mwh++;
	}
	return 0;
}



/* return the number of unsuccessful tests */
int main(pip_fpinfo* bootinfo){
	atests=0;
	tests= 0;

  log1("Test loading :\n",-1);

  log1("Pip BootInfo: \n",-1);
	parse_bootinfo(bootinfo);

	log1("Initializing paging.\n",-1);
  if(bootinfo->magic==FPINFO_MAGIC)
	   initPaging((void*)bootinfo->membegin, (void*)bootinfo->memend);
  else{
    puts("Boot init Error ");
    PANIC();
  }
	AllocAll();

	log1("PART0",-1);
	tests++;
  if(createPartition(BTVS[0],BTVS[1],BTVS[2],BTVS[3],BTVS[4])){
    log1("Partition created :",1);
		currchild=1;
    atests++;
  }
  else
    log1("Partition created :",0);

	puthex(BTVS[0]);
	puts("\n");
	puthex(BTVS[1]);
	puts("\n");
	puthex(BTVS[2]);
	puts("\n");
	puthex(BTVS[3]);
	puts("\n");
	puthex(BTVS[4]);
	puts("\n");

	log1("Creating Test Partitions : ",-1);
	log1("PART 1",-1);
	tests++;
	if (!createPartition(BTVS[0],BTVS[5],BTVS[6],BTVS[7],BTVS[8])){
		log1("Malfunctionous Partition couldn't be created (Recall of a previous used address without freeing memmory) :",1);
		atests++;
	}
	else
		log1("Malfunctionous Partition could be created (Recall of a previous used address without freeing memmory) :",0);

	log1("PART 2",-1);
	tests++;
	if(!createPartition(0x20000,BTVS[9],BTVS[10],BTVS[11],BTVS[12])){
		log1("Malfunctionous Partition coulnd't be created (Call of a non-allocated address in a non-paged area) :",1);
		atests++;
	}
	else
		log1("Malfunctionous Partition could be created (Call of a non-allocated address in a non-paged area) :",0);

	log1("Initializing interrupts... ",-1);
	Pip_RegisterInterrupt(33, &timerAsm,(uint32_t *) 0x2000000);
	Pip_RegisterInterrupt(88, &takebackAsm,(uint32_t *) 0x2001000);
	puts("Mapping partition ");
	length= fpart.end-fpart.start;

	puthex(length);
	puts("\n");
	for(offset = 0; offset < length; offset+=0x1000){
		if (mapPageWrapper((uint32_t)(fpart.start+ offset), BTVS[0],(uint32_t)(0x700000 +offset))){
			log1("Failed to map fpart ",-1);
			puthex(fpart.start + offset);
			puts(" to destination ");
			puthex(0x700000 + offset);
			puts("\n");
		}
		else{
			log1("mapped",-2);
			puthex((uint32_t)(fpart.start+ offset));
			putc('\n');
		}
	}

	puts("Mapping interrupt stack... ");
	uint32_t istack_addr = (uint32_t)allocPage();
	if(mapPageWrapper((uint32_t)istack_addr, (uint32_t)BTVS[0], (uint32_t)0x804000))
	{
		puts("Couldn't map stack.\n");
	} else {
		puts("done.\n");
	}
	puts("Mapping stack... \n");
	uint32_t stack_addr = (uint32_t)allocPage();
	puts("No alloc problems \n");
	if(mapPageWrapper((uint32_t)stack_addr,BTVS[0], (uint32_t)0xC0000000))
	{
		log1("Couldn't map stack.\n",-1);
	} else {
		puts("stack done.\n");
	}
	vidt_t * vidt = (vidt_t*) allocPage();
	puts("vidt at ");
	puthex((uint32_t)vidt);
	puts("\n");
	vidt->vint[0].eip = 0x700000 ;
	vidt->vint[0].esp = 0xC0000000 + 0x1000 - sizeof(uint32_t);
	vidt->flags = 0x1;

	if(mapPageWrapper((uint32_t)vidt,BTVS[0], (uint32_t)0xFFFFF000))
	{
		log1("Couldn't map VIDT.\n",-1);
	} else {
		puts("vidt done.\n");
	}
	multiWrapp((uint32_t *) &BTVS[13],5,BTVS[0],(uint32_t)0xA0000000);
	/*Pip_VCLI();*/

	testOnPages(BTVS[0],0x10000000);
	addVAddrTest(BTVS[0]);
  Pip_Notify((uint32_t)BTVS[0],0,0,0);
	log1("tests #0",-1);
	log1("trying to resume deleted child ",-1);
	Pip_Resume(BTVS[0],1);

	/*Test on Dispatch */
	log1("testing Dipatch ",-1);
	if (createPartition(BTVS[13],BTVS[14],BTVS[15],BTVS[16],BTVS[17])){
		log1("Created part on Dispatch",-1);
	}
	else{
		log1("Couldn't create the dispatch testPart",-1);
	}

	dispatch(BTVS[13],0,0,0);
	log1("end of tests on dispatch",-1);

	/* Time to test Count/prepare/collect */
	puts(" v0.01\n");
	log1("Achevied ",-2);
	putdec(atests);
	puts(" of ");
	putdec(tests);
	puts(" tests\n");
	puts("#AT#TOKEN");
	putdec(tests-atests);
	putc('\n');
	return (tests-atests);
}
