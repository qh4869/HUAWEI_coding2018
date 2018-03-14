#ifndef __FLAVORCOLLECT_H__
#define __FLAVORCOLLECT_H__

#include "flavor.h"
#include "linux_list.h"

typedef struct {
    int key, count;
    struct list_head nodeptr;
}collect_node_t,*CollectNode;

typedef struct {
    Flavor flavor;
    struct list_head head;
}flavor_collect_t, *FlavorCollect;

typedef struct {
    FlavorCollect collects;
    int size;
    collect_node_t * _buf;
    int _bufSize;
}flavor_collect_list_t ,*FlavorCollectList;

FlavorCollectList collectFlavorByDay(FlavorList vmlist, TDList tdlist);

#endif

