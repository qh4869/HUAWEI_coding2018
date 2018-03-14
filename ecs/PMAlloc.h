#ifndef __PMALLOC_H__
#define __PMALLOC_H__

#include "predict.h"

typedef struct {
    PHYmachine pm_info; // to add other
    int restCPU;
    int restMEM; // MB
    struct {
        Flavor flavor;
        int count;
    } vms[MAX_FLAVOR_TYPE];
} pm_alloc_info_t,*PMAllocInfo;

typedef struct {
    pm_alloc_info_t * pms;
    int size;
    int capacity;
}pm_list_t,*PMList;

PMList newPMList(int capacity);
int PMList_createPM(PMList pml);
int PMList_PMAddFlavor(PMList pml, Flavor fl, int pm_idx);
int PMList_sprintf(PMList pml, char *outs);

#define PMList_foreach(pmainfo,pml, index) \
    for(index=0, pmainfo=(pml)->pms + index;\
            index < (pml)->size; \
            index +=1, pmainfo=(pml)->pms+index)
#endif
