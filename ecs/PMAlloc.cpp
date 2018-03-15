#include "PMAlloc.h"
#include <stdio.h>
#include <string.h>

PMList newPMList(int capacity)
{
    int size = sizeof(pm_list_t) +sizeof(pm_alloc_info_t)*capacity;
    PMList pml = (PMList) malloc(size);
    memset(pml,0,size);
    pml->pms = (pm_alloc_info_t *)(pml + 1);
    pml->size = 0;
    pml->capacity = capacity;

    return pml;
}

/** create new PM and initial it
 * return new PM's idx */
int PMList_createPM(PMList pml)
{
    // ASSERT(pml->size < pml->capacity - 1);
    int size = pml->size;
    pml->pms[size].pm_info = PM_getPM();
    pml->pms[size].restCPU = PM_getPM()->cpuNumber;
    pml->pms[size].restMEM = PM_getPM()->memSize;
    // memset(&pml->pms[size].vms,0,sizeof(..)*MAX_FLAVOR_TYPE)
    pml->size++;
    return size;
}

/**
 * add flavor to pms[pm_idx]
 * return -1 if can not add 
 * return >=0  else
 * */
int PMList_PMAddFlavor(PMList pml, Flavor fl, int pm_idx)
{
    //ASSERT(pm_idx < pml->size);
    if(pm_idx >= pml->size) { return -1; }
    pm_alloc_info_t *pm;
    pm = pml->pms + pm_idx;
    int cpu = fl->cpuNumber,
        mem = fl->memSize;
    if(pm->restCPU < cpu || pm->restMEM < mem) { return -2; } 

    pm->restCPU -= cpu;
    pm->restMEM -= mem;
    int fl_idx = fl->idx;
    if(NULL == pm->vms[fl_idx].flavor) {
        pm->vms[fl_idx].flavor = fl;
    }
    pm->vms[fl_idx].count++;
    return pm->restCPU;
}

int PMList_sprintf(PMList pml, char *outs)
{
    int chars; char *oldouts = outs;
    chars = sprintf(outs,"%d\n",pml->size);
    outs += chars;
    int index; PMAllocInfo pm;
    PMList_foreach(pm,pml,index) {
       chars = sprintf(outs,"%d",index+1);
       outs += chars;
       for(int i=0; i<MAX_FLAVOR_TYPE;i++) {
           if(NULL == pm->vms[i].flavor || pm->vms[i].count <=0){continue;}
           chars = sprintf(outs," %s %d",
                   pm->vms[i].flavor->flavorType, 
                   pm->vms[i].count);
           outs += chars;
       }
       *outs = '\n'; outs++;
    }
    return outs - oldouts;
}
