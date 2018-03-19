#include<malloc.h>
/** 计算方程组 R*a = b 的解，r 指向 R 的第一行*/
int levinson(double *r, double *b, double *a, int m)
{
    double *p =(double*) malloc(m*sizeof(double)*4);
    double *curA = p, *preA = p+m, *curZ = p+2*m, *preZ = p+3*m;


    curA[0] = b[0]/r[0]; curZ[0] = r[1]/r[0];
    for(int k = 2; k<=m ; k++) {
        double *t;
        t = curA; curA = preA; preA = t;
        t = curZ; curZ = preZ; preZ = t;

        double sum = 0;
        for(int i=0;i< k-1; i++) { sum += r[i+1] * preZ[k-2-i]; }
        double zk1 = r[k] - sum;
        sum = 0;
        for(int i=0;i< k-1; i++) { sum += r[i+1] * preZ[i]; }
        double zk2 = r[0] - sum;

        double zk = zk1/zk2;
        for(int i=0;i<k-1; i++) { curZ[i] = preZ[i] - preZ[k-2-i]*zk; }
        curZ[k-1] = zk;

        sum = 0;
        for(int i=0;i< k-1; i++) { sum += r[i+1] * preA[k-2-i]; }
        double ak = b[k-1] - sum;
        ak /= zk2;

        for(int i=0;i<k-1; i++) { curA[i] = preA[i] - preZ[k-2-i]*ak; }
        curA[k-1] = ak;
    }
    for(int i=0; i< m; i++) {
        a[i] = curA[i];
    }
    free(p);
    return m;
}
