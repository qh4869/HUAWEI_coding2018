#ifndef __TRAINDATAlIST_H__
#define __TRAINDATAlIST_H__

#include <time.h>
#include "flavor.h"
typedef struct _TrainDataItem {
    // int key;
    Flavor flavor;
    time_t  createTime;
    int     createDay;
} *TDItem;

typedef struct _TrainDataList {
    TDItem list;
    int firstDay;
    int lastDay;
    int size;
    int capacity;
} *TDList;



TDList newTDList(int capacity);
void freeTDList(TDList p);
int TDList_addItem(TDList p, Flavor flavorType, time_t createTime);

#define TDList_foreach(item,alist,index) \
    for(index=0, item = (alist)->list+index; \
         index < (alist)->size; \
         index = index+1,item=(alist)->list+index)

#endif
