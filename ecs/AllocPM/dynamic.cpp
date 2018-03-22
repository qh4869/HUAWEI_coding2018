#include "predict.h"
#include "common.h"
#include "iostream"
using namespace std;

int knapsack(const int *W1, const int *W2, const int *V, int *res,  const int n, const int C1, const int C2)

{
   int value = 0;
   int ***f = new int**[n];//二维动态规划value函数,三维数组
   for (int i = 0; i < n; i++)
   {
       f[i] = new int*[C1 + 1];
       for (int j = 0; j < C1 + 1; j++)
       {
           f[i][j] = new int[C2 + 1];
       }
   }

   for (int i = 0; i < n; i++)
       for (int j = 0; j <= C1; j++)
           for (int k = 0; k <= C2; k++)
               f[i][j][k] = 0;

   for (int i = 0; i < n; i++)
   {
       f[i][0][0] = 0;
   }
   for (int i = 1; i <= C1; i++)
       for (int j = 1; j <= C2; j++)
           f[0][i][j] = (i < W1[0] && j<W2[0]) ? 0 : V[0];//动态规划最底层

   for (int i = 1; i < n; i++)
   {
       for (int j = 1; j <= C1; j++)
       {
           for (int k = 1; k <= C2; k++)
           {
               if (j >= W1[i] && k >= W2[i])
               {
                   f[i][j][k] = (f[i - 1][j][k] >(f[i - 1][j - W1[i]][k - W2[i]] + V[i])) ? 
                       f[i - 1][j][k] : (f[i - 1][j - W1[i]][k - W2[i]] + V[i]);
               }
               else {
                   f[i][j][k] = f[i - 1][j][k];
               }
           }
       }
   }
   value = f[n - 1][C1][C2];
   int p = n - 1; //层
   int q = C1; // 第一个代价
   int r = C2; // 第二个代价
   while (p>0) // 从顶层之底层搜索最优解对应的一个分配方案（其实结果可能不唯一，这里只选了一个
   {
       if (q >= W1[p] && r >= W2[p] && f[p][q][r] == (f[p - 1][q - W1[p]][r - W2[p]] + V[p]))
       {
           res[p] = 1;
           q = q - W1[p];
           r = r - W2[p];
       }
       p--;
   }
   if (f[0][q][r])
   {
       res[0] = 1;
   }

   // 释放内存
   for (int i = 0; i < n; i++)
   {
       for (int j=0; j<C1; j++)
           delete[] f[i][j];
       //f[i] = 0;
   }
   for (int i=0; i<n; i++)
       delete[] f[i];
   delete[] f;
   //f = 0;
   return value;
}

int totalLeft(int* Flavorleft, int size)
{
    int total = 0;
    for (int i=0;i<size;i++)
    {
        total += Flavorleft[i];
    }
    return total;
}

PMList flavor_alloc_to_PM(FlavorIntST st, OPTTYPE opt)
{
    int totalNum = FlavorIntST_sumAll(st);
    PMList pml = newPMList(totalNum);
    int n = totalNum;
    int C1, C2;
    int pm_id = 0;
    int *W1 = new int[totalNum];//cpu
    int *W2 = new int[totalNum];//mem
    int *V = new int[totalNum];
    //int *W1 = (int*)malloc(totalNum*sizeof(int));
    //int *W2 = (int*)malloc(totalNum*sizeof(int));
    //int *V = (int*)malloc(totalNum*sizeof(int));
        
    int *res = new int[totalNum];
    int i;//循环变量
    int iAdd;
    Flavor fl;
    int idx;
    int *FlavorLeft = new int[st->_size];
    int *FlavorLeftTmp = new int[st->_size];
    FlavorIntST_foreachKey(fl,st,idx){
        FlavorLeft[idx] = *(FlavorIntST_get(st,fl));
    }
    int number;
    while ((n=totalLeft(FlavorLeft, st->_size)) > 0)
    {
        for (i = 0; i < totalNum; i++)
        {
            res[i] = 0;
        }
        pm_id = PMList_createPM(pml);
        C1 = pml->pms->pm_info->cpuNumber;
        C2 = pml->pms->pm_info->memSize / 1024;
        iAdd = 0;
        FlavorIntST_foreachKey(fl,st,idx){ 
            number = FlavorLeft[idx];
            for (i = 0; i < number; i++)
            {
                W1[i+iAdd] = fl->cpuNumber;
                W2[i+iAdd] = fl->memSize / 1024;
                V[i+iAdd] = (fl->cpuNumber)*(fl->memSize/1024);//待优化
            }
            i = 0;
            iAdd += number;
        }
        iAdd = 0;
        int value = knapsack(W1, W2, V, res, n, C1, C2);
        printf("===========print=================\n");
        cout << "value=" << value << endl;
        cout << "n=" << n << endl;
        for (i = 0; i < n; i++)
           cout << res[i] << " ";
        cout << endl;
        printf("===========print end=============\n");
        for (i=0;i<st->_size;i++)
        {
            FlavorLeftTmp[i] = FlavorLeft[i];
        }
        for (i = 0; i < n; i++)//res=1表示这台虚拟机已被这个物理服务器选中
        {
            if (res[i]==1)
            {
                int numberAdd = 0;
                //// find fl
                //FlavorIntST_foreachKey(fl,st,idx){
                //    numberAdd += st->_map[idx].value;
                //    if (numberAdd >= i+1)
                //        break;
                //} 
                //// find idx-->j
                //for (int j = 0; j<st->_size;j++)
                //{
                //    if (st->_map[j].key == fl)
                //        FlavorLeft[j] -= 1;
                //}
                int j = 0;
                for (j=0; j<st->_size;j++)
                {
                    numberAdd += FlavorLeftTmp[j];
                    if (numberAdd >= i+1)
                        break;
                }
                FlavorLeft[j] -= 1;
                fl = st->_map[j].key;
                PMList_PMAddFlavor(pml,fl,pm_id);
            }
        }
    }
     
    delete []res;
    delete []W1;
    delete []W2;
    delete []V;
    //free(W1);
    //free(W2);
    //free(V);
    delete []FlavorLeft;
 
    return pml;
}
