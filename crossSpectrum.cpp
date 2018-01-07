
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void FFT( double *xr, double *xi, double *Xr, double *Xi, int N )
{
  int i, j, k, n, n2;
  double theta, wr, wi;

  static double *rbuf, *ibuf;
  static int    bufsize = 0;

  /* memory allocation for buffers */
  if( bufsize != N ) {
    bufsize = N;
    rbuf = (double*)calloc( sizeof(double), bufsize );
    ibuf = (double*)calloc( sizeof(double), bufsize );
  }

  /* bit reverse of xr[] & xi[] --> store to rbuf[] and ibuf[] */
  i = j = 0 ;
  rbuf[j] = xr[j];  ibuf[j] = xi[j];
  for( j = 1 ; j < N-1 ; j++ ) {
    for( k = N/2 ; k <= i ; k /= 2 )  i -= k;
    i += k;
    rbuf[j] = xr[i];  ibuf[j] = xi[i];
  }
  rbuf[j] = xr[j];  ibuf[j] = xi[j];

  /* butterfly calculation */
  theta = -2.0*M_PI;
  for( n = 1 ; ( n2 = n*2 ) <= N ; n = n2 ) {
    theta *= 0.5;
    for ( i = 0 ; i < n ; i++ ) {
      wr = cos(theta*i);  wi = sin(theta*i);
      for ( j = i ; j < N ; j += n2 ) {
	k = j + n;
	/*
	  X[j] = 1*buf[j] + W*buf[k];
	  X[k] = 1*buf[j] - W*buf[k];
	  Note : X[], buf[], and W are complex numbers.
	  Re{ X[n] } = Xr[n], Im{ X[n] } = Xi[n];
	  Re{ buf[n] } = rbuf[n], Im{ buf[n] } = ibuf[n];
	  Re{ W } = wr, Im{ W } = wi;
	*/
	Xr[j] = rbuf[j] + wr * rbuf[k] - wi * ibuf[k];  /* ??????????, using wr, wi, rbuf, and ibuf */
	Xi[j] = ibuf[j] + wr * ibuf[k] + wi * rbuf[k];  /* ??????????, using wr, wi, rbuf, and ibuf */
	Xr[k] = rbuf[j] - wr * rbuf[k] + wi * ibuf[k];  /* ??????????, using wr, wi, rbuf, and ibuf */
	Xi[k] = ibuf[j] - wr * ibuf[k] - wi * rbuf[k];  /* ??????????, using wr, wi, rbuf, and ibuf */
      }
    }

    for( i = 0 ; i < N ; i++ ) {
      rbuf[i] = Xr[i];
      ibuf[i] = Xi[i];
    }
  }
  return;
}

void IFFT( double *Xr, double *Xi, double *xr, double *xi, int N )
{
  int i, j, k, n, n2;
  double theta, wr, wi;

  static double *rbuf, *ibuf;
  static int    bufsize = 0;

  /* memory allocation for buffers */
  if( bufsize != N ) {
    bufsize = N;
    rbuf = (double*)calloc( sizeof(double), bufsize );
    ibuf = (double*)calloc( sizeof(double), bufsize );
  }

  /* bit reverse of Xr[] & Xi[] --> store to rbuf[] and ibuf[] */
  i = j = 0 ;
  rbuf[j] = Xr[j]/N;  ibuf[j] = Xi[j]/N;
  for( j = 1 ; j < N-1 ; j++ ) {
    for( k = N/2 ; k <= i ; k /= 2 )  i -= k;
    i += k;
    rbuf[j] = Xr[i]/N;  ibuf[j] = Xi[i]/N;
  }
  rbuf[j] = Xr[j]/N;  ibuf[j] = Xi[j]/N;

  /* butterfly calculation */
  theta = 2.0*M_PI;  /* not -2.0*M_PI !!! */
  for( n = 1 ; ( n2 = n*2 ) <= N ; n = n2 ) {
    theta *= 0.5;
    for ( i = 0 ; i < n ; i++ ) {
      wr = cos(theta*i);  wi = sin(theta*i);
      for ( j = i ; j < N ; j += n2 ) {
	k = j + n;
	/*
	  x[j] = 1*buf[j] + W*buf[k];
	  x[k] = 1*buf[j] - W*buf[k];
	  Note : x[], buf[], and W are complex numbers.
	  Re{ x[n] } = xr[n], Im{ x[n] } = xi[n];
	  Re{ buf[n] } = rbuf[n], Im{ buf[n] } = ibuf[n];
	  Re{ W } = wr, Im{ W } = wi;
	*/
	xr[j] = rbuf[j] + wr * rbuf[k] - wi * ibuf[k];
	xi[j] = ibuf[j] + wr * ibuf[k] + wi * rbuf[k];
	xr[k] = rbuf[j] - wr * rbuf[k] + wi * ibuf[k];
	xi[k] = ibuf[j] - wr * ibuf[k] - wi * rbuf[k];
      }
    }

    for( i = 0 ; i < N ; i++ ) {
      rbuf[i] = xr[i];
      ibuf[i] = xi[i];
    }
  }
  return;
}

int main( int argc, char *argv[] )
{
  int i, framelen = 8;
  short  *sdata;
  double *x1i, *X1r, *X1i;
  double *x2i, *X2r, *X2i;
  double *yr, *yi, *Yr, *Yi;


  /* memory allocation */
  sdata = (short*)calloc( sizeof(short), framelen );
  double x1r[] = {1, 4, 1, 1, 1, 1, 1, 1};
  x1i = (double*)calloc( sizeof(double), framelen );
  X1r = (double*)calloc( sizeof(double), framelen );
  X1i = (double*)calloc( sizeof(double), framelen );
  double x2r[] = {1, 1, 4, 1, 1, 1, 1, 1};
  x2i = (double*)calloc( sizeof(double), framelen );
  X2r = (double*)calloc( sizeof(double), framelen );
  X2i = (double*)calloc( sizeof(double), framelen );
  yr = (double*)calloc( sizeof(double), framelen );
  yi = (double*)calloc( sizeof(double), framelen );
  Yr = (double*)calloc( sizeof(double), framelen );
  Yi = (double*)calloc( sizeof(double), framelen );

  //Fourier transform
  FFT( x1r, x1i, X1r, X1i, framelen );
  FFT( x2r, x2i, X2r, X2i, framelen );

  for (i = 0; i < framelen; i++) {
    //make complex conjugate of X1
    X1i[i] *= -1;
  }

  for (i = 0; i < framelen; i++) {
    //make cross spectrum
    Yr[i] = X1r[i] * X2r[i] - X1i[i] * X2i[i];
    Yi[i] = X1r[i] * X2i[i] + X1i[i] * X2r[i];
  }

  //make cross-correlation function
  IFFT( Yr, Yi, yr, yi, framelen );

  printf("Cross-correlation\n");
  for( i = 0 ; i < framelen ; i++ ) {
    printf( "%d %lf\n", i, yr[i] );
  }

  /*
  for( i = 0 ; i < framelen ; i++ ) {
    printf( "%d %lf\n", i, xr[i] );
  }
  */

  return( 0 );
}
