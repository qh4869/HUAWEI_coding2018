#ifndef __FLAVOR_H__
#define __FLAVOR_H__

#define MAX_FLAVOR_NAME_LEN  (10+1)
#define MAX_FLAVOR_ID_LEN  (36+1)
typedef struct _flavor_t {
    int idx;  // flavor id in this process
    // char flavorID[40];
    char flavorType[MAX_FLAVOR_NAME_LEN]; // 虚拟机规格
    int cpuNumber;
    int memSize;  // MB
} *Flavor;

typedef struct _flavor_list_t {
    struct _flavor_t * flavorFly;
    int size;
    int capacity;
} *FlavorList;

FlavorList newFlavorList(int capacity);
void freeFlavorList(FlavorList l);
int FlavorList_addType(FlavorList list, char *flavorType,int cpuNumber, int memSize);
Flavor FlavorList_getFlavorByType(FlavorList list, char *flavorType);

#define FlavorList_foreach(item,alist,index) \
    for(index=0, item = (alist)->flavorFly+index; \
         index < (alist)->size; \
         index = index+1,item=(alist)->flavorFly+index)

#endif
