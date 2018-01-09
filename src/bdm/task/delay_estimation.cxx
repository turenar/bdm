#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FRAMELEN 16384

void FFT(double* xr, double* xi, double* Xr, double* Xi, int N) {
	int i, j, k, n, n2;
	double theta, wr, wi;

	static double* rbuf, * ibuf;
	static int bufsize = 0;

	/* memory allocation for buffers */
	if (bufsize != N) {
		bufsize = N;
		rbuf = (double*) calloc(sizeof(double), bufsize);
		ibuf = (double*) calloc(sizeof(double), bufsize);
	}

	/* bit reverse of xr[] & xi[] --> store to rbuf[] and ibuf[] */
	i = j = 0;
	rbuf[j] = xr[j];
	ibuf[j] = xi[j];
	for (j = 1; j < N - 1; j++) {
		for (k = N / 2; k <= i; k /= 2) { i -= k; }
		i += k;
		rbuf[j] = xr[i];
		ibuf[j] = xi[i];
	}
	rbuf[j] = xr[j];
	ibuf[j] = xi[j];

	/* butterfly calculation */
	theta = -2.0 * M_PI;
	for (n = 1; (n2 = n * 2) <= N; n = n2) {
		theta *= 0.5;
		for (i = 0; i < n; i++) {
			wr = cos(theta * i);
			wi = sin(theta * i);
			for (j = i; j < N; j += n2) {
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

		for (i = 0; i < N; i++) {
			rbuf[i] = Xr[i];
			ibuf[i] = Xi[i];
		}
	}
	return;
}

void IFFT(double* Xr, double* Xi, double* xr, double* xi, int N) {
	int i, j, k, n, n2;
	double theta, wr, wi;

	static double* rbuf, * ibuf;
	static int bufsize = 0;

	/* memory allocation for buffers */
	if (bufsize != N) {
		bufsize = N;
		rbuf = (double*) calloc(sizeof(double), bufsize);
		ibuf = (double*) calloc(sizeof(double), bufsize);
	}

	/* bit reverse of Xr[] & Xi[] --> store to rbuf[] and ibuf[] */
	i = j = 0;
	rbuf[j] = Xr[j] / N;
	ibuf[j] = Xi[j] / N;
	for (j = 1; j < N - 1; j++) {
		for (k = N / 2; k <= i; k /= 2) { i -= k; }
		i += k;
		rbuf[j] = Xr[i] / N;
		ibuf[j] = Xi[i] / N;
	}
	rbuf[j] = Xr[j] / N;
	ibuf[j] = Xi[j] / N;

	/* butterfly calculation */
	theta = 2.0 * M_PI;  /* not -2.0*M_PI !!! */
	for (n = 1; (n2 = n * 2) <= N; n = n2) {
		theta *= 0.5;
		for (i = 0; i < n; i++) {
			wr = cos(theta * i);
			wi = sin(theta * i);
			for (j = i; j < N; j += n2) {
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

		for (i = 0; i < N; i++) {
			rbuf[i] = xr[i];
			ibuf[i] = xi[i];
		}
	}
	return;
}

int getDelay(short* sig1, short* sig2) {
	int i;
	short* sdata;
	double* x1r, * x1i, * X1r, * X1i;
	double* x2r, * x2i, * X2r, * X2i;
	double* yr, * yi, * Yr, * Yi;

	/* memory allocation */
	sdata = (short*) calloc(sizeof(short), FRAMELEN);
	x1r = (double*) calloc(sizeof(double), FRAMELEN);
	x1i = (double*) calloc(sizeof(double), FRAMELEN);
	X1r = (double*) calloc(sizeof(double), FRAMELEN);
	X1i = (double*) calloc(sizeof(double), FRAMELEN);
	x2r = (double*) calloc(sizeof(double), FRAMELEN);
	x2i = (double*) calloc(sizeof(double), FRAMELEN);
	X2r = (double*) calloc(sizeof(double), FRAMELEN);
	X2i = (double*) calloc(sizeof(double), FRAMELEN);
	yr = (double*) calloc(sizeof(double), FRAMELEN);
	yi = (double*) calloc(sizeof(double), FRAMELEN);
	Yr = (double*) calloc(sizeof(double), FRAMELEN);
	Yi = (double*) calloc(sizeof(double), FRAMELEN);
/*
  fseek( fpDAT, 0, SEEK_SET );
  fread( sdata, sizeof(short), framelen, fpDAT );
*/
	for (i = 0; i < FRAMELEN; i++) {
		x1r[i] = (0.54 - 0.46 * cos(2 * M_PI * i / (FRAMELEN - 1))) * sig1[i];
		x1i[i] = 0.0;
	}
/*
  fseek( fpDAT, 20*sizeof(short), SEEK_SET );
  fread( sdata, sizeof(short), framelen, fpDAT );
  fclose( fpDAT );
*/
	for (i = 0; i < FRAMELEN; i++) {
		x2r[i] = (0.54 - 0.46 * cos(2 * M_PI * i / (FRAMELEN - 1))) * sig2[i];
		x2i[i] = 0.0;
	}

	//Fourier transform
	FFT(x1r, x1i, X1r, X1i, FRAMELEN);
	FFT(x2r, x2i, X2r, X2i, FRAMELEN);

	for (i = 0; i < FRAMELEN; i++) {
		//make complex conjugate of X1
		X1i[i] *= -1;
	}

	for (i = 0; i < FRAMELEN; i++) {
		//make cross spectrum
		Yr[i] = X1r[i] * X2r[i] - X1i[i] * X2i[i];
		Yi[i] = X1r[i] * X2i[i] + X1i[i] * X2r[i];
	}

	//make cross-correlation function
	IFFT(Yr, Yi, yr, yi, FRAMELEN);
/*
  for( i = 0 ; i < FRAMELEN ; i++ ) {
    printf( "%d %lf\n", i, yr[i] );
  }
*/
	//estimate time shift
	double max = 0;
	int peek = 0;
	for (i = 0; i < FRAMELEN; i++) {
		if (yr[i] > max) {
			peek = i;
			max = yr[i];
		}
	}
	//printf( "time shift : %d\n", (peek + FRAMELEN/2)%FRAMELEN - FRAMELEN/2 );
	int delay = (peek + FRAMELEN / 2) % FRAMELEN - FRAMELEN / 2;

	return (delay);
}
