#ifndef _TEST_H
#define _TEST_H

#define PANIC() for(;;)

void log1(char* str,int tok);
void parse_bootinfo(pip_fpinfo* bootinfo);
int testOnPages(uint32_t part,uint32_t area);
int MultAddrTest(uint32_t part);
void DumpValue(uint32_t add);
#endif
