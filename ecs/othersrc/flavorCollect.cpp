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
    int size = vmlist->size;
    FlavorCollectList fcs = newFlavorCollectList(vmlist, size*(tdlist->lastDay - tdlist->firstDay + 1));

    int *flCount = (int *)malloc(size*sizeof(int));
    memset(flCount,0,size*sizeof(int));

    int curDay =tdlist->firstDay;
    int prevDay = curDay;
    TDItem item; int i;
    CollectNode node;
    Flavor fl; int i_fl;

    TDList_foreach(item,tdlist,i){
        int day = item->createDay;
        int idx = item->flavor->idx;
        if(day == prevDay) {
            flCount[idx]++;
        } else {
            for(;curDay<day;curDay++) {
                FlavorList_foreach(fl,vmlist,i_fl) {
                    node = FlavorCollectList_mallocNode(fcs);
                    node->key = curDay;
                    node->count = flCount[i_fl];
                    list_add_tail(&node->nodeptr, &fcs->collects[i_fl].head);
                }
                memset(flCount,0,size*sizeof(int));
            }
            flCount[idx]++;
            prevDay = day;
        }
    }

    return fcs;
}


