#include "estimate.h"
#include "fft.h"
#include "stdlib.h"
#define SAMPLETIME 0.1

int main(int argc, char const *argv[]) {
  //prepare sample sound data
  short *sigdata1, *sigdata2, *sigdata3, *sigdata4;
  FILE *fpDAT;

  if( NULL == ( fpDAT = fopen( "a.raw", "r" ) ) )  exit( 1 );
  sigdata1 = (short*)calloc( sizeof(short), FRAMELEN );
  sigdata2 = (short*)calloc( sizeof(short), FRAMELEN );
  sigdata3 = (short*)calloc( sizeof(short), FRAMELEN );
  sigdata4 = (short*)calloc( sizeof(short), FRAMELEN );
  fseek( fpDAT, 15*sizeof(short), SEEK_SET );
  fread( sigdata1, sizeof(short), FRAMELEN, fpDAT );
  fseek( fpDAT, 0*sizeof(short), SEEK_SET );
  fread( sigdata2, sizeof(short), FRAMELEN, fpDAT );
  fseek( fpDAT, 0*sizeof(short), SEEK_SET );
  fread( sigdata3, sizeof(short), FRAMELEN, fpDAT );
  fseek( fpDAT, 5*sizeof(short), SEEK_SET );
  fread( sigdata4, sizeof(short), FRAMELEN, fpDAT );
  fclose( fpDAT );
  //preparation end
  double T1, T2, T3;
  T1 = getDelay(sigdata1, sigdata2) * SAMPLETIME;
  T2 = getDelay(sigdata1, sigdata3) * SAMPLETIME;
  T3 = getDelay(sigdata1, sigdata4) * SAMPLETIME;
  double *p = (double*)malloc(sizeof(double) * 3);
  estimatePosition(0, T1, T2, T3, p);
  printf("%lf %lf %lf\n", p[0], p[1], p[2]);
  return 0;
}
