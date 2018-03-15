#ifndef __FLAVORINTST_H__
#define __FLAVORINTST_H__
#include "predict.h"

typedef struct _flavor_int_st_t{
    struct {
        Flavor key;
        int value;
    } _map[MAX_FLAVOR_TYPE];
    int _size;
    int _capacity;
}*FlavorIntST;

FlavorIntST newFlavorIntST();
int FlavorIntST_put(FlavorIntST st,Flavor key, int value);
int *FlavorIntST_get(FlavorList st, Flavor key);
/***************************************************/
int FlavorIntST_sumAll(FlavorIntST st);
int FlavorIntST_sprintf(FlavorIntST st, char *outs);
#endif

