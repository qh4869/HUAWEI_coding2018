#ifndef __PHYMACHINE_H__
#define __PHYMACHINE_H__

typedef struct _PHYmachine {
    int cpuNumber;
    int memSize; // MB
    // int diskSize; // GB
} *PHYmachine;

PHYmachine PM_init(char *str);
PHYmachine PM_getPM();
#endif
