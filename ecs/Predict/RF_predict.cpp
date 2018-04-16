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
        int no = 0;
        double totalFlNum = 0;
		sscanf(fl->flavorType, "flavor%d", &no);//get flavor number for different predict 
        switch(no)
        {
        	case 1:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 2.35));// 2.35-4.35down2 2.35->1.35down0.4 2.35->1.85same
        		printf("no:%d\n",no);
        		break;
        	case 2:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 4.16));// 2.76->3.76 up0.3 3.76->5.76down2 
        		printf("no:%d\n",no);									//2.76-3.16 same 3.16->4.16 final78.187
        		break;
        	case 3:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 3.01));//2.7 2.7->14.7 same no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 4:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.25));//0 2.8 2.8->1.8 same no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 5:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.3));// 1.7->3.7 down2 1.7->0.7 down0.3 1.7->1.2 same
        		printf("no:%d\n",no);
        		break;
        	case 6:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 5));//1.8 1.8->3.8 no this flavor 
        		printf("no:%d\n",no);
        		break;
        	case 7:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 0.61));//1.6 1.6->2.6 no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 8:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.9));// 2.1->3.8 down2 3.8->1.4 down0.13 
        		printf("no:%d\n",no);
        		break;
        	case 9:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.5)); // 1.2->0.6 down0.3 0.6->2.5 down2
        		printf("no:%d\n",no);
        		break;
        	case 10:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.25));//1.3 no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 11:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 7.2));// 1.6->3.2 up0.3 3.2->3.7 up0.4 3.7->4.7 up0.3 
        		printf("no:%d\n",no);									//4.7->5.6 up 0.3 5.6-6.6 up0.3 6.9->7.2 same
        		break;
        	case 12:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 8.69));//8.69 1.8 0->2 no diff no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 13:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.25));//1.6 1.3->1.6 no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 14:
        		totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.25));//2.7 1.7->2.7 no this flavor
        		printf("no:%d\n",no);
        		break;
        	case 15:
	        	totalFlNum = predict(num_vs_day,totalDay,preDays);
		    	FlavorIntST_put(st, fl, (int)(totalFlNum + 0.5 + 1.25));//1.9 1.9->10.9 no this flavor
        		printf("no:%d\n",no);
        		break;
        	default:
        		printf("No this flavor!\n");
        	break;
        }
        
	}

	free(fcl);
	return st;
}
