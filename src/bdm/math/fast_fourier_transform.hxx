#pragma once

#include <complex>
#include <memory>

namespace bdm::math {
	class fast_fourier_transform {
	public:
		using complex_type = std::complex<double>;

		void fft(std::size_t size, const complex_type* in, complex_type* out);
		void ifft(std::size_t size, const complex_type* in, complex_type* out);

	private:
		std::unique_ptr<complex_type[]> _buf;
		std::size_t _buf_size = 0;

		void allocate_buffer(std::size_t size);
	};
}
