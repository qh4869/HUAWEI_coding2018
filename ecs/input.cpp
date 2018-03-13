#include <stdio.h>
#include <string.h>
#include <time.h>
#include "flavor.h"
#include "trainDataList.h"

int readFlavor(FlavorList list, char *info[])
{
    int len,i;
    int cpuNumber,memSize;
    char flavorType[MAX_FLAVOR_NAME_LEN];

    sscanf(info[0],"%d\n",&len);
    info++;
    for(i=0;i<len;i++){
        sscanf(info[i],"%s %d %d\n",flavorType,&cpuNumber,&memSize);
        FlavorList_addType(list, flavorType,cpuNumber,memSize);
    }
    return len+1;
}

time_t readTime(char *str) 
{
    struct tm  timep;
    memset(&timep,0,sizeof(timep));
    sscanf(str,"%d-%d-%d %d:%d:%d\n",
            &timep.tm_year, &timep.tm_mon, &timep.tm_mday,
            &timep.tm_hour, &timep.tm_min, &timep.tm_sec);
    timep.tm_year -= 1900;
    timep.tm_mon -= 1;
    return mktime(&timep);
}

int readTrainData(char *data[],int data_num, TDList dlist, FlavorList flist)
{
    int line;
    char flavorID[100],flavorType[20],date[100],time[30]; 
    Flavor fl; time_t t;
    for(line=0; line < data_num; line++){
       sscanf(data[line],"%s\t%s\t%s %s\n",flavorID,flavorType,date,time);
       fl = FlavorList_getFlavorByType(flist,flavorType);
       if(NULL == fl) { continue; }
       strcat(date," "); strcat(date,time);
       t = readTime(date);
       TDList_addItem(dlist,fl,t);
    }
    return line;
}
