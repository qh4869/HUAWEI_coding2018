#include <limits.h>
#include "common.h"
#include "predict.h"

int flCmpCPU(Flavor fl1,Flavor fl2)
{
    if(fl1->cpuNumber != fl2->cpuNumber) {
        return fl2->cpuNumber - fl1->cpuNumber;
    } else {
        return fl1->memSize - fl2->memSize;
    }
    return 0;
}
int flCmpMEM(Flavor fl1,Flavor fl2)
{
    if(fl1->memSize != fl2->memSize) {
        return fl2->memSize - fl1->memSize;
    } else {
        return fl1->cpuNumber - fl2->cpuNumber;
    }
    return 0;
}

void insectionSort(Flavor *base,int n,int (*cmpfl)(Flavor,Flavor))
{
    for(int i = 0; i<n ; i++)
        for(int j=i; j>0 && cmpfl(base[j],base[j-1]) < 0 ; j--) {
            Flavor t = base[j];
            base[j] = base[j-1];
            base[j-1] = t;
        }
    
}

PMList flavor_alloc_to_PM(FlavorIntST st, OPTTYPE opt)
{
    int totalNum = FlavorIntST_sumAll(st);
    PMList pml = newPMList(totalNum);

    Flavor fl;int i;
    int maxR = INT_MIN ,minR = INT_MAX;
    Flavor fls[MAX_FLAVOR_TYPE+1]; 
    int flsCount[MAX_FLAVOR_TYPE+1];
    int flsNum = 0;
    FlavorIntST_foreachKey(fl,st,i) {
        fls[flsNum] = fl;
        flsNum++;
        int t = fl->memSize/fl->cpuNumber;
        if(t>maxR) { maxR = t; }
        if(t<minR) { minR = t; }
    }

    PHYmachine pm = PM_getPM();
    double mem2cpu = pm->memSize/(double)pm->cpuNumber;
    if(mem2cpu > maxR) { opt = CPU; }
    else if(mem2cpu < minR) { opt = MEM; }
    if(opt == CPU) {
        insectionSort(fls,flsNum,flCmpCPU);
    }else {
        insectionSort(fls,flsNum,flCmpMEM);
    }

    for(i = 0; i<flsNum; i++) {
        fl = fls[i];
        int *pValue = FlavorIntST_get(st,fl);
        flsCount[i] = pValue == NULL? 0 :(*pValue);
    }
    
    int pm_id = 0,bDone = 0;
    while(!bDone){
        bDone = 1;
        for(i=0; i<flsNum; i++) {
            while(flsCount[i] > 0) {
                int suc = PMList_PMAddFlavor(pml,fls[i],pm_id);
                if(suc >= 0) { flsCount[i]--; }
                else { bDone = 0; break; }
            }
        }
        if(!bDone) {
            pm_id = PMList_createPM(pml);
        }
    }
    return pml;
}
