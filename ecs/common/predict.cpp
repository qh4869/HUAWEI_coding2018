#include "common.h"
#include "predict.h"
#include "input.h"

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

    // PHYmachine pm = PM_getPM();
	char * result_file = (char *)malloc(data_num*50);
    *result_file='\0'; char *outs = result_file;
    /**
     *@var PHYmachine   pm           �������Ϣ
     *@var FLavorList   vmlist       ��ҪԤ���������б�
     *@var string       optType      �Ż����� "CPU" or "MEM"
     *@var time_t startTime, endTime ҪԤ���ʱ�俪ʼ�ͽ���
     *@var TDList        tdlist      ѵ������ 
     *@var char*         outs        ����ֽ���
     */

    // to do
    OPTTYPE opt;
    if(0 == strcmp(optType,"CPU")) {opt=CPU;} else {opt=MEM;}
    FlavorIntST st_flavor_num = flavor_predict(vmlist, tdlist,startTime,endTime);
    int totalNum = FlavorIntST_sumAll(st_flavor_num);
    PMList pml = flavor_alloc_to_PM(st_flavor_num,opt);
    

	// ��Ҫ���������
    int chars;
    chars = sprintf(outs,"%d\n",totalNum); outs += chars; 
    chars = FlavorIntST_sprintf(st_flavor_num,outs); outs +=chars;
    *outs = '\n'; outs++; // add null line
    chars = PMList_sprintf(pml,outs); outs += chars;
	write_result(result_file, filename);

    // =============
    free(pml); free(st_flavor_num);
    free(result_file);
    free(vmlist); free(tdlist);
}
