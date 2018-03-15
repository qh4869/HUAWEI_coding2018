#include <malloc.h>
#include <string.h>
#include "trainDataList.h"

/**
 * 训练数据列表
 * */
TDList newTDList(int capacity)
{
    TDList p = (TDList)malloc(capacity*sizeof(struct _TrainDataItem)
                                +sizeof(struct _TrainDataList));
    p->list = (TDItem)(p+1);
    p->size = 0;
    p->capacity = capacity;
    return p;
}

void freeTDList(TDList p) { free(p); }

#define SECS_PER_DAY (60*60*24)
int TDList_addItem(TDList p, Flavor flavor, time_t createTime)
{
   p->list[p->size].flavor = flavor;
   p->list[p->size].createTime = createTime;
   p->list[p->size].createDay = (int)(createTime/SECS_PER_DAY);
   p->size++;
   return p->size;
}
