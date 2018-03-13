#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "flavor.h"

/** 虚拟机规格 */

FlavorList newFlavorList(int capacity){
    FlavorList list = (FlavorList)malloc(sizeof(struct _flavor_t)*capacity
            + sizeof(_flavor_list_t));
    list->flavorFly = (Flavor)(list+1);
    list->size = 0;
    list->capacity = capacity;
    return list;
}
void freeFlavorList(FlavorList l) { free(l); }

int FlavorList_addType(FlavorList list, char *flavorType,int cpuNumber, int memSize)
{
    int size = list->size;
    strcpy(list->flavorFly[size].flavorType,flavorType);
    list->flavorFly[size].cpuNumber = cpuNumber;
    list->flavorFly[size].memSize = memSize;
    list->size++;
    return list->size;
}

Flavor FlavorList_getFlavorByType(FlavorList list, char *flavorType)
{
    int i;
    for(i=0;i<list->size;i++) {
        if(0 == strcmp(list->flavorFly[i].flavorType,flavorType)) {
            return list->flavorFly + i;
        }
    }
    return NULL;    
}

