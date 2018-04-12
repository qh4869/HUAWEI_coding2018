#include "common.h"
#include "predict.h"
#include "flavorCollect.h"
#include "denoise.h"

int levinson(double *R, double *b, double *a, int len);
int autocorr(int *x,double *corr, int n)
{
    for(int i=0;i<n;i++) {
        double sum = 0; int count = 0;
        for(int j=0;j+i<n;j++) {
            sum += x[j] * x[j+i];
            count++;
        }
        corr[i] = sum/count;
    }
    return n;
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

#define ARORDER  17
    double *a = (double *)malloc(ARORDER*sizeof(double));
    FlavorList_foreach(fl,vmlist, idx) {
         CollectNode pos;
         struct list_head *ptr,*head=&(fcl->collects[idx].head);
         for(int i=0;i<corr_size;i++) { corr[i] = 0; }
         int days = 0;
         for(ptr=head->next; ptr !=head ; ptr=ptr->next) {
             pos = container_of(ptr,collect_node_t,nodeptr);
             num_vs_day[days++] = pos->count;
         }

         autocorr(num_vs_day,corr,totalDay);
         double totalFlNum = 0;
         for(int day = 1; day <=preDays; day++) {
            levinson(corr,corr+day,a,ARORDER);
            double dayTotal = 0;
            for(int j=0;j<ARORDER;j++) {
                dayTotal += a[j] * num_vs_day[totalDay-1-j];
            }
            totalFlNum += dayTotal > 0? dayTotal:0;
         }
         FlavorIntST_put(st,fl,(int)(totalFlNum+0.5));
    }

    free(fcl);
    return st;
}
