#include "predict.h"
#include <stdio.h>
#include "input.h"
#include "flavorCollect.h"
#include "string.h"

//��Ҫ��ɵĹ��������
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

    //PHYmachine pm = PM_getPM();
    
    /**
     *@var PHYmachine   pm           �������Ϣ
     *@var FLavorList   vmlist       ��ҪԤ���������б�
     *@var string       optType      �Ż����� "CPU" or "MEM"
     *@var time_t startTime, endTime ҪԤ���ʱ�俪ʼ�ͽ���
     *@var TDList        tdlist      ѵ������ 
     */
    
    // to do
    
    FlavorCollectList fcs = collectFlavorByDay(vmlist,tdlist);

    int i;
    Flavor fl;
    double diff = difftime(endTime,startTime);
    int numOfDaysToPredict = (int)diff/3600/24;
    char flavorIDTmp[100],flavorTypeTmp[20],dateTmp[100],timeTmp[30]; 
    sscanf(data[0],"%s\t%s\t%s %s\n",flavorIDTmp,flavorTypeTmp,dateTmp,timeTmp);
    strcat(dateTmp," ");
    strcat(dateTmp, timeTmp);
    int trainStartDay = readTime(dateTmp) / (3600*24);
    sscanf(data[data_num-1],"%s\t%s\t%s %s\n",flavorIDTmp,flavorTypeTmp,dateTmp,timeTmp);
    strcat(dateTmp," ");
    strcat(dateTmp, timeTmp);
    int trainEndDay = readTime(dateTmp) / (3600*24);
    int numOfDaysToTrain = trainEndDay - trainStartDay + 1;
    printf("===========predict(average)==================\n");
    printf("flavorType\tpredictNum\n");
    vmlist->predictTotalNum = 0;
    FlavorList_foreach(fl,vmlist,i) {
        int idx = fl->idx;
        CollectNode pos; 
        struct list_head *ptr,*head = &(fcs->collects[idx].head);
        int tmpSum = 0;
        for(ptr = head->next; ptr !=head; ptr = ptr->next) {
            pos = container_of(ptr,collect_node_t, nodeptr);
            tmpSum += pos->count;
            
        }
        double predictNum = (double)tmpSum / numOfDaysToTrain * numOfDaysToPredict;       
        fl->predictNum = (int)(predictNum+0.5);
        vmlist->predictTotalNum += fl->predictNum;
        printf("%s\t%d\n", fl->flavorType, fl->predictNum);
    }

    //========================================================

  //  printf("========PHY Info===================\n");
  //  printf("PHY: %d %dMB\n",pm->cpuNumber,pm->memSize);
  //  printf("========Flavor Info===================\n");
  //  int i;
  //  Flavor fl;
  //  FlavorList_foreach(fl,vmlist,i) {
  //      printf("%s %d %d\n", fl->flavorType, fl->cpuNumber, fl->memSize);
  //  }
  //  double diff = difftime(endTime,startTime);
  //  printf("startTime:%ld endTime:%ld diff:%f\n",
  //          startTime, endTime, diff/3600/24);
  //  printf("========training data===============\n");
  //  printf("flavor type\tcreateDay\tcreateTime\n");
  //  TDItem item; 
  //  TDList_foreach(item,tdlist,i) {
  //      printf("%s\t%d\t%ld\n",
  //              item->flavor->flavorType,
  //              item->createDay,
  //              item->createTime);
  //  }

  //  printf("========training data Collects===============\n");
  //  FlavorList_foreach(fl,vmlist,i) {
  //      int idx = fl->idx;
  //      printf("Flavor Type:%s\n",fcs->collects[idx].flavor->flavorType);
  //      CollectNode pos; 
  //      struct list_head *ptr,*head = &(fcs->collects[idx].head);
  //      for(ptr = head->next; ptr !=head; ptr = ptr->next) {
  //          pos = container_of(ptr,collect_node_t, nodeptr);
  //          printf("%d,%d\n",pos->key,pos->count);
  //      }
  //  }


	// ��Ҫ���������
	//char * result_file = (char *)"17\n\n0 8 0 20";
    char result_file[50000];
    char buffer[100];
    sprintf(result_file, "%d\n", vmlist->predictTotalNum);
    FlavorList_foreach(fl,vmlist,i) { 
        sprintf(buffer, "%s %d\n", fl->flavorType, fl->predictNum);
        strcat(result_file, buffer);
     }
    strcat(result_file, "\n");// ����

	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	write_result(result_file, filename);
    free(fcs);
    free(vmlist);
    free(tdlist);
}
