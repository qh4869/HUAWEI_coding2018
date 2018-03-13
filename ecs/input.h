#ifndef __INPUT_H__
#define __INPUT_H__

#include <time.h>
#include "trainDataList.h"
int readFlavor(FlavorList list, char *info[]);
time_t readTime(char *str);
int readTrainData(char *data[],int data_num, TDList dlist, FlavorList flist);

#endif
