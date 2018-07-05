#include <stdint.h>
#include <pip/fpinfo.h>
#include <pip/paging.h>
#include <pip/vidt.h>
#include <pip/api.h>
#include <pip/compat.h>
#include <pip/debug.h>
#include "test.h"
#define BUFFSIZE 20 /* size of the buffers */
#define TTTEST 10 /* number of tets executed */


void main(){
  log1("Generated part #0",-1);
  Pip_Notify(0,88,0,0);
  for(;;){
  }
}
