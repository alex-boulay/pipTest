#include <stdint.h>
#include <pip/fpinfo.h>
#include <pip/paging.h>
#include <pip/vidt.h>
#include <pip/api.h>
#include <pip/compat.h>
#include <pip/debug.h>
#include "test.h"

int *tests,*atests;

void main(){

  tests=(int *)0x73C000;
  atests=(int *)0x73C004;

  //puthex(*tests);putc('\n');
  //puthex(*atests);putc('\n');

  /* Test of writing on transmited area
   * First one is writable second one isn't
   */

  *tests= *tests+1;
  log1(" on Child fpart : test writing on writable area - 0x73A000 ",-1);

  if(* (int *)0x73A000 = 43){
    log1("Succeded to write on writable area ",1);
    *atests = *atests+1;
  }
  else {
    log1("Failed to write on writable area ",0);
  }


  *tests = *tests+1;
  if(!(*(int *)0x73B000 = 43)){
    log1("Couldn't write on unwritable area ",1);
    *atests = *atests+1;
  }
  else {
    log1("Succeded to write on unwritable area ",0);
  }
  

  log1("Generated part #0",-1);
  Pip_Notify(0,88,0,0);
  for(;;){
  }
}
