#include <pip/debug.h>
#include <pip/compat.h>
#include <pip/fpinfo.h>
#include "test.h"

void log1(char* str,int tok)
{
    puts("|TEST| ");
    puts(str);
    if(tok==0)
      puts( "--Failed");
    else if(tok==1)
      puts(" --Worked");
    else
      putc('\n');
    return;
}
