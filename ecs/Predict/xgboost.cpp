#include "common.h"
#include "predict.h"
#include "flavorCollect.h"

int  xgboostlinear(int *inputy,int n, int m)
{
    int *tx = (int *) malloc(sizeof(int)*n);
    double *ty = (double *) malloc(sizeof(double)*n);
    for(int i=0; i<n ;i++) { tx[i] = i; ty[i] = inputy[i]; }
    
    double a = 0,b = 0;
    for(int round=0; round<10; round++) {
        int sx2 = 0, sx=0;
        double sxy = 0,sy=0;
        for(int i=0; i<n; i++) {
            sx2 += tx[i]*tx[i]; sx += tx[i];
            sxy += tx[i]*ty[i]; sy += ty[i];
        }
        double ha = (n*sxy - sx*sy)/(n*sx2-sx*sx);
        double hb = (sy - ha * sx)/n;
        a += ha;
        b += hb;
        printf("%f,%f,%d\n",b,a,round);
        for(int i=0; i<n; i++) {
            double predy = b + a*i;
            ty[i] = inputy[i] - predy;
        }
    }

    double sump = 0;
    for(int i=n; i<n+m; i++) {
       double p = a*i+b;
       p = p>=0?p:0;
       sump += p; 
    }
    free(tx); free(ty); 
    return (int)(sump+0.5);
}

FlavorIntST flavor_predict(FlavorList vmlist, TDList tdlist, time_t startTime, time_t endTime)
{
    FlavorIntST st = newFlavorIntST();
    Flavor fl; int idx;
    FlavorList_foreach(fl,vmlist,idx) {
        FlavorIntST_put(st,fl,0);
    }

    FlavorCollectList fcl =  collectFlavorByDay(vmlist, tdlist);

    int totalDay = tdlist->lastDay - tdlist->firstDay + 1;
    double preDays = difftime(endTime, startTime)/3600/24;

    int *num_vs_day = (int *) malloc(totalDay*sizeof(int));
    int corr_size = totalDay < 40? 40:totalDay;
    double *corr = (double *) malloc(corr_size*sizeof(double));
    printf("%d\n",totalDay);

    FlavorList_foreach(fl,vmlist, idx) {
         CollectNode pos;
         struct list_head *ptr,*head=&(fcl->collects[idx].head);
         for(int i=0;i<corr_size;i++) { corr[i] = 0; }
         int days = 0;
         for(ptr=head->next; ptr !=head ; ptr=ptr->next) {
             pos = container_of(ptr,collect_node_t,nodeptr);
             num_vs_day[days++] = pos->count;
         }
         // num_vs_day 每天的个数, totalDay 总天数
         // preDays 要预测的天数
         int totalFlNum = 0;

         totalFlNum = xgboostlinear(num_vs_day,totalDay,preDays);

         FlavorIntST_put(st,fl,totalFlNum);
    }

    free(fcl);
    return st;
}
