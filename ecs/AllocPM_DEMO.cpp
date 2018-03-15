
#include "predict.h"
PMList flavor_alloc_to_PM(FlavorIntST st, int totalNum)
{
    PMList pml = newPMList(totalNum);
    Flavor fl; int pm_id = 0;
    for(int i=0; i<st->_size; i++) {
        fl = st->_map[i].key;
        int num = st->_map[i].value;
        for(int j=0;j<num;j++) {
            // 添加 num 次 fl 到 pm_id 若失败则创建新的 pm 并添加
            while(PMList_PMAddFlavor(pml,fl,pm_id) < 0) {
                pm_id = PMList_createPM(pml);
            }
        }
    }
    return pml;
}
