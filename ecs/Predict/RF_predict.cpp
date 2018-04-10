#include "common.h"
#include "predict.h"
#include "flavorCollect.h"
#include "denoise.h"

double predict(double *num_vs_day, int totalDay, int preDays);
FlavorIntST flavor_predict(FlavorList vmlist, TDList tdlist, time_t startTime, time_t endTime)
{
	FlavorIntST st = newFlavorIntST();
	Flavor fl; int idx;
	FlavorList_foreach(fl, vmlist, idx) {
		FlavorIntST_put(st, fl, 0);
	}

	FlavorCollectList fcl = collectFlavorByDay(vmlist, tdlist);

	int totalDay = tdlist->lastDay - tdlist->firstDay + 1;
	double preDays = difftime(endTime, startTime) / 3600 / 24;

	double *num_vs_day = (double *)malloc(totalDay * sizeof(double));
	int corr_size = totalDay < 40 ? 40 : totalDay;
	double *corr = (double *)malloc(corr_size * sizeof(double));
	printf("%d\n", totalDay);

	FlavorList_foreach(fl, vmlist, idx) {
		CollectNode pos;
		struct list_head *ptr, *head = &(fcl->collects[idx].head);
		for (int i = 0; i<corr_size; i++) { corr[i] = 0; }
		int days = 0;
		for (ptr = head->next; ptr != head; ptr = ptr->next) {
			pos = container_of(ptr, collect_node_t, nodeptr);
			num_vs_day[days++] = pos->count;
		}
        // -----------denoising---------------
        //denoising_x2(num_vs_day, totalDay);
        //denoising_LOF(num_vs_day, totalDay);
        //denoising_fft(num_vs_day, totalDay);

        // -----------predict----------------
        double totalFlNum = 0;
        totalFlNum = predict(num_vs_day,totalDay,preDays);
		FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5));
	}

	free(fcl);
	return st;
}
