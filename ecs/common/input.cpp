#include "input.h"

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
    time_t diff;
    memset(&timep,0,sizeof(timep));
    timep.tm_year=70; timep.tm_mday=1;
    diff = mktime(&timep);
    sscanf(str,"%d-%d-%d %d:%d:%d\n",
            &timep.tm_year, &timep.tm_mon, &timep.tm_mday,
            &timep.tm_hour, &timep.tm_min, &timep.tm_sec);
    timep.tm_year -= 1900;
    timep.tm_mon -= 1;
    return mktime(&timep) - diff;
}

#define SECS_PER_DAY (60*60*24)
int readTrainData(char *data[],int data_num, TDList dlist, FlavorList flist)
{
    int line;
    char flavorID[100],flavorType[20],date[100],time[30]; 
    Flavor fl; time_t t;
    // FirstDay
   sscanf(data[0],"%s\t%s\t%s %s\n",flavorID,flavorType,date,time);
   strcat(date," "); strcat(date,time);
   t = readTime(date);
   dlist->firstDay = t/SECS_PER_DAY;
   dlist->lastDay = -1;
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
