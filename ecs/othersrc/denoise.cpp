#include "denoise.h"

/*-----denoising卡方统计量--------*/
void denoising_x2(int* num_vs_day, const int totalDay)
{
    int totalNum = 0;
    double ave = 0;// average
    double x2[totalDay];// x^2
    for (int i=0; i<totalDay; i++)
    {
        totalNum += num_vs_day[i];
    }
    ave = (double)totalNum / totalDay;
    for (int i=0; i<totalDay; i++)
    {
        x2[i] = pow(num_vs_day[i]-ave, 2) / ave;
    }
    for (int i=0; i<totalDay; i++)
    {
        if (x2[i] > THREADHOLD_x2) // 异常
            num_vs_day[i] = (int)(ave+0.5); // 填充平均值
    } 
}

/*-----------------sort and max------------------------*/
int cmp1(const void *a, const void *b) 
{ 
     return(*(int *)a-*(int *)b);  //升序 
}
int cmp2(const void *a, const void *b) 
{ 
     return(*(int *)b-*(int *)a); //降序 
}
void qsort ( void * base, size_t num, size_t size, int ( * comparator ) ( const void *, const void * ) );
#define max(a,b) ((a)>(b)?(a):(b))

/*-----denoising Local Outlier Factor(LOF)--------*/
void denoising_LOF(int* num_vs_day, const int totalDay)
{
    int diff[totalDay];
    int KDis[totalDay];// k-distance
    int rDist[totalDay][totalDay];// rechability distance
    int neighbor[totalDay][totalDay];
    double lrd[totalDay];// local rechability density
    double lof[totalDay];// LOF value
    int totalNum = 0;
    double ave = 0;// average
    for (int i=0; i<totalDay; i++)
    {
        totalNum += num_vs_day[i];
    }
    ave = (double)totalNum / totalDay;
    for (int i=0; i<totalDay; i++)
    {
        for (int j=0; j<totalDay; j++)
        {
            diff[j] = abs(num_vs_day[i] - num_vs_day[j]);
        }
        qsort(diff, totalDay, sizeof(int), cmp1);
        KDis[i] = diff[K];
        if (KDis[i] == 0)
            KDis[i] = 1;// too many zeros
    }
    for (int i=0; i<totalDay; i++)
    {
        for (int j=0; j<totalDay; j++)
        {
            diff[j] = abs(num_vs_day[i] - num_vs_day[j]);
            rDist[j][i] = max(KDis[i], diff[j]);
        }
    }
    for (int i=0; i<totalDay; i++)
    {
        for (int j=0; j<totalDay; j++)
        {
            diff[j] = abs(num_vs_day[i] - num_vs_day[j]);
            neighbor[j][i] = diff[j]<=KDis[i]?1:0;
        }
    }
    for (int i=0; i<totalDay; i++)
    {
        double add = 0;
        int cnt = 0;
        for (int j=0; j<totalDay; j++)
        {
            if (neighbor[j][i]  == 1)
            {
                add += rDist[j][i];
                cnt += 1;
            }  
        }
        lrd[i] = 1 / (add/cnt);
    }
    for (int i=0; i<totalDay; i++)
    {
        double add = 0;
        int cnt = 0;
        for (int j=0; j<totalDay; j++)
        {
            if (neighbor[j][i]  == 1)
            {
                add += lrd[j];
                cnt += 1; 
            }
        }
        lof[i] = (add/cnt) / lrd[i];
    }
    for (int i=0; i<totalDay; i++)
    {
        if (lof[i] > THREADHOLD_LOF) // 异常
            num_vs_day[i] = (int)(ave+0.5); // 填充平均值
            // num_vs_day[i] = 0;
    } 
}

/*------------------------fft----------------------*/
void denoising_fft(int* num_vs_day, const int totalDay)
{
    complex* fftData = (complex*)malloc(FFTSIZE*sizeof(complex));
    int2complex(num_vs_day, fftData, totalDay, FFTSIZE);
    fft(FFTSIZE,fftData);
    for (int i=CUT; i<FFTSIZE-1-CUT; i++)
    {
        fftData->real = 0;
        fftData->imag = 0;
    }
    ifft(FFTSIZE,fftData);
    complex2int(fftData, num_vs_day, totalDay, FFTSIZE);

    free(fftData);
}