#include "common.h"
#include "predict.h"
#include "flavorCollect.h"

FlavorIntST flavor_predict(FlavorList vmlist, TDList tdlist, time_t startTime, time_t endTime)
{
    FlavorIntST st = newFlavorIntST();
    Flavor fl; int idx;
    FlavorList_foreach(fl,vmlist,idx) {
        FlavorIntST_put(st,fl,0);
    }
    TDItem td; int i;
    TDList_foreach(td,tdlist,i) {
        int *p = FlavorIntST_get(st,td->flavor);
        if(NULL != p) { (*p)++; }
    }

    FlavorCollectList fcl =  collectFlavorByDay(vmlist, tdlist);
    
    FlavorList_foreach(fl,vmlist, idx) {
         int idx = fl->idx;
         printf("Flavor Type:%s\n",fcl->collects[idx].flavor->flavorType);
         CollectNode pos;
         struct list_head *ptr,*head=&(fcl->collects[idx].head);
         for(ptr=head->next; ptr !=head ; ptr=ptr->next) {
             pos = container_of(ptr,collect_node_t,nodeptr);
             printf("%d\t%d\n",pos->key,pos->count);
         }
    }
    return st;
}
