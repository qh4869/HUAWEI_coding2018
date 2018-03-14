#ifndef __ROUTE_H__
#define __ROUTE_H__

#include <malloc.h>
#include "lib_io.h"
#include "flavor.h"
#include "phymachine.h"
#include "trainDataList.h"
#define MAX_FLAVOR_TYPE 15

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);

	

#endif
