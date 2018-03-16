#include <string.h>
#include <malloc.h>
#include "predict.h"
#include "flavorCollect.h"

FlavorCollectList newFlavorCollectList(FlavorList vmlist, int dtSize)
{
    int size = vmlist->size;
    int totalSize = sizeof(flavor_collect_list_t) +
                    sizeof(flavor_collect_t) * size +
                    sizeof(collect_node_t) * dtSize;
    void *buf = malloc( totalSize);
    memset(buf,0,totalSize);
    FlavorCollectList fclist = (FlavorCollectList)buf;
    fclist->collects = (FlavorCollect)(fclist + 1);
    fclist->size = size;
    fclist->_buf=(collect_node_t *)(fclist->collects + size);
    fclist->_bufSize=0;

    int i; Flavor fl;
    FlavorList_foreach(fl,vmlist,i) {
        fclist->collects[fl->idx].flavor = fl;
        fclist->collects[fl->idx].head 
            = LIST_HEAD_INIT( fclist->collects[fl->idx].head );
    }

    return fclist;
}

CollectNode FlavorCollectList_mallocNode(FlavorCollectList fc)
{
    int size = fc->_bufSize;
    fc->_bufSize++;
    return fc->_buf + size;
}

FlavorCollectList collectFlavorByDay(FlavorList vmlist, TDList tdlist)
{ 
    FlavorCollectList fcs = newFlavorCollectList(vmlist, tdlist->size);
    int size = fcs->size;
    int *prevDay = (int *)malloc(2*size*sizeof(int)),
        *flCount = prevDay + size;
    memset(prevDay,-1,size*sizeof(int));
    memset(flCount,0,size*sizeof(int));
    int i;
    TDItem item;
    CollectNode node; 
    TDList_foreach(item,tdlist,i){
        int idx = item->flavor->idx;
        int day = item->createDay;
        if(-1 == prevDay[idx] || day == prevDay[idx]) {
            flCount[idx]++;
        }else{
            node = FlavorCollectList_mallocNode(fcs);
            node->key = prevDay[idx];
            node->count = flCount[idx];
            list_add_tail(&node->nodeptr, &fcs->collects[idx].head);
            flCount[idx] = 1;
        }
        prevDay[idx] = day;
    }
    Flavor fl;
    FlavorList_foreach(fl,vmlist,i) {
        int idx = fl->idx;
        if(flCount[idx]==0) { continue; }
        node = FlavorCollectList_mallocNode(fcs);
        node->key = prevDay[idx];
        node->count = flCount[idx];
        list_add_tail(&node->nodeptr, &fcs->collects[idx].head);
    }
    return fcs;
}


