#include"reg_RF.h"
#include <cmath>
#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;
//generate a random permutated sequence(length N)
int *reperm(const int N)
{
	int *a = (int *)malloc(N * sizeof(int));
	int temp, x;
	//srand((unsigned)time(NULL));//ÖØÉèËæ»úÖÖ×Ó
	for (int i = 0; i < N; i++)
		a[i] = i;

	for (int i = 0; i < N; i++)
	{
		//printf("%d\n", a[i]);
		temp = a[i];
		x = rand() % N;
		a[i] = a[x];
		a[x] = temp;
	}
	//swap(a[i], a[rand() % (N+1)]);

	return a;
}

//predict function
double predict(double *num_vs_day, int totalDay, int preDays)
{
	double *eachday = (double *)malloc(preDays * sizeof(double));
	const int col = 10;//È¡Ô¤²âµÄÇ°Ê®ÌìÊý¾Ý×÷ÎªÏà¹ØÓ°ÏìÒòËØ
					   //const int N = totalDay - col;
	const int N = totalDay - col;
	double(*X)[col] = (double(*)[col])malloc(sizeof(double)* N * col);
	double *Y = (double *)malloc(N * sizeof(double));
	double(*X_temp)[col] = (double(*)[col])malloc(sizeof(double) * N * col);
	double *X_trn = (double *)malloc(sizeof(double) * N * col);
	double *Y_trn = (double *)malloc(N * sizeof(double));
	double *X_tst = (double *)malloc(col * sizeof(double));
	double ypred = 0;//Ô¤²âÒ»ÌìµÄ½á¹û ³õÊ¼»¯Îª0
	int index;
	int *repermRow;


	for (int i = 0; i < N; i++) {
		Y[i] = num_vs_day[i + col];

		for (int j = 0; j<col; j++) {
			X[i][j] = num_vs_day[i + j];
		}
	}
	repermRow = reperm(N);//permutated the row sequence randomly
	for (int i = 0; i < N; i++) {
		index = repermRow[i];
		Y_trn[i] = Y[index];
		//printf("%f\n", Y_trn[i]);
		for (int j = 0; j < col; j++) {
			X_temp[i][j] = X[index][j];
		}
	}
	for (int i = 0; i < N; i++) { //µÈ¼ÛÓÚMATLABÖÐµÄ×ªÖÃ
		for (int j = 0; j < col; j++) {
			X_trn[i*col + j] = X_temp[i][j];
			//printf("%f	%d\n", X_trn[i*col + j], i*col + j);
		}
	}


	for (int i = 0; i < col; i++) {
		X_tst[i] = num_vs_day[totalDay - col + i];
		//printf("%f	", X_tst[i]);
	}
	extern int in_regTree; //// 0 -initialize and normal.  1-normal  , -99 release
	extern int in_findBestSplit; // 0 -initialize and normal.  1-normal  , -99 release
	for (int i = 0; i < preDays; i++) {
		in_regTree = 0;
		in_findBestSplit = 0;
		ypred = rfPredict(N, col, X_trn, Y_trn, X_tst);
		for (int i = 0; i < col - 1; i++) {
			X_tst[i] = X_tst[i + 1];
		}
		X_tst[col - 1] = ypred;

		eachday[i] = ypred;
	}

	double totalFlNum = 0;
	for (int day = 0; day < preDays; day++) {
		totalFlNum += eachday[day];
	}
	return totalFlNum;
}

double rfPredict(int N, int col, double *X_trn, double *Y_trn, double *X_tst) {
	//input parameters
	int sampsize = N;
	int xdim[2] = { N,col };
	int nthsize = 5;//5 for regression 1 for classification
	int nrnodes = 2 * (int)((float)floor((float)(sampsize / (1>(nthsize - 4) ? 1 : (nthsize - 4))))) + 1;
	const int NT = 1800;
	int nTree = NT;//the default number of trees
	int mtry = 2;
	int imp[3] = { 1,0,3 };
	int cat[10] = { 1,1,1,1,1,1,1,1,1,1 };
	int maxcat = 1;
	int jprint = 0;
	int doProx = 0;
	int oobprox = 0;
	int biasCorr = 0;
	const int keepf[2] = { 1,0 };
	int replace = 1;
	//output parameters   yptr->ypred errimp->impout
	double  impmat, impSD, prox, mse[NT], xts, yts, yTestPred, proxts = 1, coef[2];
	int *nout = (int *)malloc(N * NT * sizeof(int));
	double *yptr = (double *)malloc(N * sizeof(double));
	double *errimp = (double *)malloc(col * sizeof(double));
	double *msets = (double *)malloc(NT * sizeof(double));
	double(*avnode)[NT] = (double(*)[NT])malloc(sizeof(double)* nrnodes * nTree);
	double(*upper)[NT] = (double(*)[NT])malloc(sizeof(double)* nrnodes * nTree);

	int(*lDaughter)[NT] = (int(*)[NT])malloc(sizeof(int)* nrnodes * nTree);
	int(*rDaughter)[NT] = (int(*)[NT])malloc(sizeof(int)* nrnodes * nTree);
	int(*mbest)[NT] = (int(*)[NT])malloc(sizeof(int)* nrnodes * nTree);
	int treeSize[NT], testdat = 0, nts = 1, labelts = 1, inbag;
	char(*nodestatus)[NT] = (char(*)[NT])malloc(sizeof(char)* nrnodes * nTree);
	/**/
	regRF(X_trn, Y_trn, xdim, &sampsize,
		&nthsize, &nrnodes, &nTree, &mtry, imp,
		cat, maxcat, &jprint, doProx, oobprox,
		biasCorr, yptr, errimp, &impmat,
		&impSD, &prox, treeSize, (char *)nodestatus,
		(int*)lDaughter, (int*)rDaughter, (double *)avnode, (int *)mbest,
		(double *)upper, mse, keepf, &replace,
		testdat, &xts, &nts, &yts, labelts,
		&yTestPred, &proxts, msets, coef,
		nout, &inbag);

	int p_size = col;
	int n_size = 1;
	int mdim = p_size;


	int keepPred = 0;
	double allPred = 0;
	double proxMat = 0;
	int nodes = 0;
	int *nodex; nodex = (int*)calloc(n_size, sizeof(int));
	double ypred = 0;//Ô¤²âÒ»ÌìµÄ½á¹û ³õÊ¼»¯Îª0
	regForest(X_tst, &ypred, &mdim, &n_size,
		&nTree, (int*)lDaughter, (int*)rDaughter,
		(char *)nodestatus, &nrnodes, (double *)upper,
		(double *)avnode, (int *)mbest, treeSize, cat,
		maxcat, &keepPred, &allPred, doProx,
		&proxMat, &nodes, nodex);
	return ypred;
}
