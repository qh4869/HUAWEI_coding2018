#include "predict.h"

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

    int totalDay = tdlist->lastDay - tdlist->firstDay + 1;
    double preDays = difftime(endTime, startTime)/3600/24;

    FlavorList_foreach(fl,vmlist,idx) {
        int *p = FlavorIntST_get(st,fl);
        (*p) = (int)((*p)*preDays / totalDay + 0.5);
    }

    return st;
}
