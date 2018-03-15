#include <stdio.h>
#include "phymachine.h"

/** 
 * 保存物理机信息
 * */
static struct _PHYmachine singletonPM[1];

PHYmachine PM_init(char *str)
{
    int cpuNumber,memSize,diskSize;
    sscanf(str,"%d %d %d\n",&cpuNumber,&memSize,&diskSize);
    memSize <<= 10; // Converter GB to MB
    singletonPM->cpuNumber = cpuNumber;
    singletonPM->memSize = memSize;
    return singletonPM;
}

PHYmachine PM_getPM()
{
    return singletonPM;
}
