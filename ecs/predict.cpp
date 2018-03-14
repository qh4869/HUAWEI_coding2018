#include "predict.h"
#include <stdio.h>
#include <string.h>
#include "input.h"
#include "flavorCollect.h"
#include "PMAlloc.h"

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    int line=0; //,flavorTypeNum;
    char optType[10];
    time_t startTime,endTime;
    PM_init(info[line++]); line++; // skip null line
    FlavorList vmlist = newFlavorList(MAX_FLAVOR_TYPE);
    line += readFlavor(vmlist,info+line); line++; // skip null line
    sscanf(info[line++],"%s\n",optType); line++; // skip null line
    startTime = readTime(info[line++]);
    endTime = readTime(info[line++]); 
    
    TDList tdlist = newTDList(MAX_DATA_NUM);
    readTrainData(data, data_num, tdlist, vmlist);

    PHYmachine pm = PM_getPM();
	char * result_file = (char *)malloc(data_num*50);
    *result_file='\0'; char *outs = result_file;
    /**
     *@var PHYmachine   pm           物理机信息
     *@var FLavorList   vmlist       需要预测的虚拟机列表
     *@var string       optType      优化类型 "CPU" or "MEM"
     *@var time_t startTime, endTime 要预测的时间开始和结束
     *@var TDList        tdlist      训练数据 
     */
    PMList pml = newPMList(data_num);
    // to do
    
    FlavorCollectList fcs = collectFlavorByDay(vmlist,tdlist);


    printf("========PHY Info===================\n");
    printf("PHY: %d %dMB\n",pm->cpuNumber,pm->memSize);
    printf("========Flavor Info===================\n");
    int i;
    Flavor fl;
    FlavorList_foreach(fl,vmlist,i) {
        printf("%s %d %d\n", fl->flavorType, fl->cpuNumber, fl->memSize);
    }
    double diff = difftime(endTime,startTime);
    printf("startTime:%ld endTime:%ld diff:%f\n",
            startTime, endTime, diff/3600/24);
    printf("========training data===============\n");
    printf("flavor type\tcreateDay\tcreateTime\n");
    TDItem item; 
    TDList_foreach(item,tdlist,i) {
        printf("%s\t%d\t%ld\n",
                item->flavor->flavorType,
                item->createDay,
                item->createTime);
    }

    printf("========training data Collects===============\n");
    FlavorList_foreach(fl,vmlist,i) {
        int idx = fl->idx;
        printf("Flavor Type:%s\n",fcs->collects[idx].flavor->flavorType);
        CollectNode pos; 
        struct list_head *ptr,*head = &(fcs->collects[idx].head);
        for(ptr = head->next; ptr !=head; ptr = ptr->next) {
            pos = container_of(ptr,collect_node_t, nodeptr);
            printf("%d,%d\n",pos->key,pos->count);
        }
    }


	// 需要输出的内容
    PMList_sprintf(pml,outs);

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
    free(result_file);
    free(fcs);
    free(vmlist);
    free(tdlist);
}
