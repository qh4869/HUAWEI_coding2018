#ifndef __FLAVORINTST_H__
#define __FLAVORINTST_H__
#include "common.h"

typedef struct _flavor_int_st_t{
    struct {
        Flavor key;
        int value;
    } _map[MAX_FLAVOR_TYPE+1];
    int _size;
    int _capacity;
}*FlavorIntST;

FlavorIntST newFlavorIntST();
int FlavorIntST_put(FlavorIntST st,Flavor key, int value);
int *FlavorIntST_get(FlavorIntST st, Flavor key);
/***************************************************/
int FlavorIntST_sumAll(FlavorIntST st);
int FlavorIntST_sprintf(FlavorIntST st, char *outs);

#define FlavorIntST_foreachKey(fl,st,idx) \
    for(idx=0,fl = (st)->_map[idx].key; \
            idx<(st->_size);\
            idx += 1, fl=(st)->_map[idx].key)
#endif

