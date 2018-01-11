#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "bdm/task/delay_estimation.hxx"

namespace bdm::task {
	namespace {
		void FFT(double* xr, double* xi, double* Xr, double* Xi, std::size_t N) {
			int i, j, k, n, n2;
			double theta, wr, wi;

			static double* rbuf, * ibuf;
			static std::size_t bufsize = 0;

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
				for (k = N / 2; k <= i; k /= 2) {
					i -= k;
				}
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

		void IFFT(double* Xr, double* Xi, double* xr, double* xi, std::size_t N) {
			int i, j, k, n, n2;
			double theta, wr, wi;

			static double* rbuf, * ibuf;
			static std::size_t bufsize = 0;

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
				for (k = N / 2; k <= i; k /= 2) {
					i -= k;
				}
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
	}

	int delay_estimation::get_delay(std::size_t buf_size,const  std::int16_t* sig1, const std::int16_t* sig2) {
		int i;
		short* sdata;
		double* x1r, * x1i, * X1r, * X1i;
		double* x2r, * x2i, * X2r, * X2i;
		double* yr, * yi, * Yr, * Yi;

		/* memory allocation */
		sdata = (short*) calloc(sizeof(short), buf_size);
		x1r = (double*) calloc(sizeof(double), buf_size);
		x1i = (double*) calloc(sizeof(double), buf_size);
		X1r = (double*) calloc(sizeof(double), buf_size);
		X1i = (double*) calloc(sizeof(double), buf_size);
		x2r = (double*) calloc(sizeof(double), buf_size);
		x2i = (double*) calloc(sizeof(double), buf_size);
		X2r = (double*) calloc(sizeof(double), buf_size);
		X2i = (double*) calloc(sizeof(double), buf_size);
		yr = (double*) calloc(sizeof(double), buf_size);
		yi = (double*) calloc(sizeof(double), buf_size);
		Yr = (double*) calloc(sizeof(double), buf_size);
		Yi = (double*) calloc(sizeof(double), buf_size);
/*
  fseek( fpDAT, 0, SEEK_SET );
  fread( sdata, sizeof(short), framelen, fpDAT );
*/
		for (i = 0; i < buf_size; i++) {
			x1r[i] = (0.54 - 0.46 * cos(2 * M_PI * i / (buf_size - 1))) * sig1[i];
			x1i[i] = 0.0;
		}
/*
  fseek( fpDAT, 20*sizeof(short), SEEK_SET );
  fread( sdata, sizeof(short), framelen, fpDAT );
  fclose( fpDAT );
*/
		for (i = 0; i < buf_size; i++) {
			x2r[i] = (0.54 - 0.46 * cos(2 * M_PI * i / (buf_size - 1))) * sig2[i];
			x2i[i] = 0.0;
		}

		//Fourier transform
		FFT(x1r, x1i, X1r, X1i, buf_size);
		FFT(x2r, x2i, X2r, X2i, buf_size);

		for (i = 0; i < buf_size; i++) {
			//make complex conjugate of X1
			X1i[i] *= -1;
		}

		for (i = 0; i < buf_size; i++) {
			//make cross spectrum
			Yr[i] = X1r[i] * X2r[i] - X1i[i] * X2i[i];
			Yi[i] = X1r[i] * X2i[i] + X1i[i] * X2r[i];
		}

		//make cross-correlation function
		IFFT(Yr, Yi, yr, yi, buf_size);
/*
  for( i = 0 ; i < FRAMELEN ; i++ ) {
    printf( "%d %lf\n", i, yr[i] );
  }
*/
		//estimate time shift
		double max = 0;
		int peek = 0;
		for (i = 0; i < buf_size; i++) {
			if (yr[i] > max) {
				peek = i;
				max = yr[i];
			}
		}
		//printf( "time shift : %d\n", (peek + FRAMELEN/2)%FRAMELEN - FRAMELEN/2 );
		int delay = (peek + buf_size / 2) % buf_size - buf_size / 2;

		return (delay);
	}
}
