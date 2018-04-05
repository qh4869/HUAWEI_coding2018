#ifndef __DENOISING_H__  
#define __DENOISING_H__  

#include <math.h>
#include <stdlib.h>
#include "fft.h"

// ----------function---------------
void denoising_x2(int* num_vs_day, const int totalDay);
void denoising_LOF(int* num_vs_day, const int totalDay);
void denoising_fft(int* num_vs_day, const int totalDay);

// -----------parameter-------------
// x2
#define THREADHOLD_x2 150 
// LOF
# define K 35
# define THREADHOLD_LOF 18
// FFT
# define CUT 125
# define FFTSIZE 256


#endif