#include "bdm/math/fast_fourier_transform.hxx"
#include "bdm/math/util.hxx"

namespace bdm::math {
	void fast_fourier_transform::fft(std::size_t size, const complex_type* in, complex_type* out) {
		assert_pow(size);

		double theta;

		allocate_buffer(size);

		/* bit reverse of xr[] & xi[] --> store to rbuf[] and ibuf[] */
		{
			std::size_t i = 0, j = 0;
			i = j = 0;
			_buf[j] = in[j];

			for (j = 1; j < size - 1; j++) {
				std::size_t k;
				for (k = size / 2; k <= i; k /= 2) { i -= k; }
				i += k;
				_buf[j] = in[i];
			}
			_buf[j] = in[j];
		}

		/* butterfly calculation */
		theta = -2.0 * pi;

		std::size_t n2;
		for (std::size_t n = 1; (n2 = n * 2) <= size; n = n2) {
			theta *= 0.5;
			for (std::size_t i = 0; i < n; i++) {
				complex_type w{cos(theta * i), sin(theta * i)};

				for (std::size_t j = i; j < size; j += n2) {
					std::size_t k = j + n;
					/*
					  X[j] = 1*buf[j] + W*buf[k];
					  X[k] = 1*buf[j] - W*buf[k];
					  Note : X[], buf[], and W are complex numbers.
					  Re{ X[n] } = Xr[n], Im{ X[n] } = Xi[n];
					  Re{ buf[n] } = rbuf[n], Im{ buf[n] } = ibuf[n];
					  Re{ W } = wr, Im{ W } = wi;
					*/
					out[j] = complex_type{
							_buf[j].real() + w.real() * _buf[k].real() - w.imag() * _buf[k].imag(),
							_buf[j].imag() + w.real() * _buf[k].imag() + w.imag() * _buf[k].real()};
					out[k] = complex_type{
							_buf[j].real() - w.real() * _buf[k].real() + w.imag() * _buf[k].imag(),
							_buf[j].imag() - w.real() * _buf[k].imag() - w.imag() * _buf[k].real()};
				}
			}

			for (std::size_t i = 0; i < size; i++) {
				_buf[i] = out[i];
			}
		}
	}

	void fast_fourier_transform::ifft(std::size_t size, const complex_type* in, complex_type* out) {
		allocate_buffer(size);

		const double dsize = size;

		{
			std::size_t i, j;
			i = j = 0;

			_buf[j] = in[j] / dsize;

			for (j = 1; j < size - 1; j++) {
				std::size_t k;
				for (k = size / 2; k <= i; k /= 2) { i -= k; }
				i += k;
				_buf[j] = in[j] / dsize;
			}

			_buf[j] = in[j] / dsize;
		}

		/* butterfly calculation */
		double theta = 2.0 * pi;
		std::size_t n, n2;
		for (n = 1; (n2 = n * 2) <= size; n = n2) {
			theta *= 0.5;
			for (std::size_t i = 0; i < size; i++) {
				complex_type w{cos(theta * i), sin(theta * i)};
				for (std::size_t j = i; j < size; j += n2) {
					std::size_t k = j + n;

					out[j] = complex_type{
							_buf[j].real() + w.real() * _buf[k].real() - w.imag() * _buf[k].imag(),
							_buf[j].imag() + w.real() * _buf[k].imag() + w.imag() * _buf[k].real()};
					out[k] = complex_type {
							_buf[j].real() - w.real() * _buf[k].real() + w.imag() * _buf[k].imag(),
							_buf[j].imag() - w.real() * _buf[k].imag() - w.imag() * _buf[k].real()};
				}
			}

			for (std::size_t i = 0; i < size; i++) {
				_buf[i] = out[i];
			}
		}
	}

	void fast_fourier_transform::allocate_buffer(std::size_t size) {
		if (_buf_size != size) {
			_buf = std::make_unique<complex_type[]>(size);
			_buf_size = size;
		}
	}
}
