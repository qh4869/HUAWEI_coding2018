#include "FlavorIntST.h"

FlavorIntST newFlavorIntST() {
    FlavorIntST st = (FlavorIntST)malloc(sizeof(struct _flavor_int_st_t));
    memset(st,0,sizeof(struct _flavor_int_st_t));
    st -> _capacity = MAX_FLAVOR_TYPE;
    return st;
}

int FlavorIntST_put(FlavorIntST st,Flavor key, int value)
{
    int i;
    for(i=0; i<st->_size; i++) {
        if(st->_map[i].key == key) { break; }
    }
    if(i>=st->_size) { st->_size++; }
    st->_map[i].key = key;
    st->_map[i].value = value;
    return 1;
}

/* 返回指向值的指针  if exsit 
 * 返回 NULL         else */
int *FlavorIntST_get(FlavorIntST st, Flavor key)
{
    int i;
    for(i=0; i<st->_size; i++) {
        if(st->_map[i].key == key) { 
            return &(st->_map[i].value);
        }
    }
    return NULL;
} 

/***************************************************/
int FlavorIntST_sumAll(FlavorIntST st)
{
    int i,sum=0;
    for(i=0; i<st->_size; i++) {
        sum += st->_map[i].value;
    }
    return sum;
}
int FlavorIntST_sprintf(FlavorIntST st, char *outs)
{
    char *oldouts = outs;
    int chars, i;
    for(i=0; i<st->_size; i++) {
        chars = sprintf(outs,"%s %d\n",
                st->_map[i].key->flavorType,
                st->_map[i].value);
        outs += chars;
    }
    return outs - oldouts;
}
