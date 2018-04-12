/**************************************************************
 * mex interface to Andy Liaw et al.'s C code (used in R package randomForest)
 * Added by Abhishek Jaiantilal ( abhishek.jaiantilal@colorado.edu )
 * License: GPLv2
 * Version: 0.02
 *
 * Supporting file that has some declarations.
 *************************************************************/

#define uint32 unsigned long
#define SMALL_INT char
#ifdef MATLAB
#define SMALL_INT_CLASS mxCHAR_CLASS //will be used to allocate memory t
#endif
// #define random(a,b) (rand()%(b-a+1)+a)
void seedMT(uint32 seed);
uint32 randomMT(void);
int* reperm(const int N);
double predict(double *num_vs_day, int totalDay, int preDays);
double rfPredict(int N, int col, double *X_trn, double *Y_trn, double *X_tst);
void regRF(double *x, double *y, int *xdim, int *sampsize,
	int *nthsize, int *nrnodes, int *nTree, int *mtry, int *imp,
	int *cat, int maxcat, int *jprint, int doProx, int oobprox,
	int biasCorr, double *yptr, double *errimp, double *impmat,
	double *impSD, double *prox, int *treeSize, SMALL_INT *nodestatus,
	int *lDaughter, int *rDaughter, double *avnode, int *mbest,
	double *upper, double *mse, const int *keepf, int *replace,
	int testdat, double *xts, int *nts, double *yts, int labelts,
	double *yTestPred, double *proxts, double *msets, double *coef,
	int *nout, int *inbag);

void regForest(double *x, double *ypred, int *mdim, int *n,
	int *ntree, int *lDaughter, int *rDaughter,
	SMALL_INT *nodestatus, int *nrnodes, double *xsplit,
	double *avnodes, int *mbest, int *treeSize, int *cat,
	int maxcat, int *keepPred, double *allpred, int doProx,
	double *proxMat, int *nodes, int *nodex);
void R_qsort_I(double *v, int *I, int i, int j);


