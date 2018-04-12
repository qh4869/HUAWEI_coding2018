#ifndef __DENOISING_H__  
#define __DENOISING_H__  

#include <math.h>
#include <stdlib.h>
#include "fft.h"

// ----------function---------------
void denoising_x2(double* num_vs_day, const int totalDay);
void denoising_LOF(double* num_vs_day, const int totalDay);
void denoising_fft(double* num_vs_day, const int totalDay);

// -----------parameter-------------
// x2
#define THREADHOLD_x2 1470 
// LOF
# define K 30
# define THREADHOLD_LOF 30
// FFT
# define CUT 110
# define FFTSIZE 256


#endif
