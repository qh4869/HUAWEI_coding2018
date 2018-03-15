#ifndef __INPUT_H__
#define __INPUT_H__

#include "common.h"

int readFlavor(FlavorList list, char *info[]);
time_t readTime(char *str);
int readTrainData(char *data[],int data_num, TDList dlist, FlavorList flist);

#endif
