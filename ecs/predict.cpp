#include "predict.h"
#include <stdio.h>
#include "input.h"
#include "flavor.h"
#include "phymachine.h"
#include "trainDataList.h"

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
    
    TDList dtlist = newTDList(MAX_DATA_NUM);
    readTrainData(data, data_num, dtlist, vmlist);

    PHYmachine pm = PM_getPM();
    /**
     *@var PHYmachine   pm           �������Ϣ
     *@var FLavorList   vmlist       ��ҪԤ���������б�
     *@var string       optType      �Ż����� "CPU" or "MEM"
     *@var time_t startTime, endTime ҪԤ���ʱ�俪ʼ�ͽ���
     *@var DTList        dtlist      ѵ������ 
     */
    
    // to do

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
    TDList_foreach(item,dtlist,i) {
        printf("%s\t%d\t%ld\n",
                item->flavor->flavorType,
                item->createDay,
                item->createTime);
    }


	// ��Ҫ���������
	char * result_file = (char *)"17\n\n0 8 0 20";

	// ֱ�ӵ�������ļ��ķ��������ָ���ļ���(ps��ע���ʽ����ȷ�ԣ�����н⣬��һ��ֻ��һ�����ݣ��ڶ���Ϊ�գ������п�ʼ���Ǿ�������ݣ�����֮����һ���ո�ָ���)
	write_result(result_file, filename);
    freeFlavorList(vmlist);
    freeTDList(dtlist);
}
