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
    
    char filename[100];
    FlavorList_foreach(fl,vmlist, idx) {
        sprintf(filename,"%s.txt",fl->flavorType);
        FILE *fp = fopen(filename,"w+");
         int idx = fl->idx;
         CollectNode pos;
         struct list_head *ptr,*head=&(fcl->collects[idx].head);
         for(ptr=head->next; ptr !=head ; ptr=ptr->next) {
             pos = container_of(ptr,collect_node_t,nodeptr);
             fprintf(fp,"%d\t%d\n",pos->key,pos->count);
         }
    }
    free(fcl);
    return st;
}
