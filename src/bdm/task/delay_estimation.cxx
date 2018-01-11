#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <memory>
#include "bdm/task/delay_estimation.hxx"
#include "bdm/math/util.hxx"

namespace bdm::task {
	namespace {
		void window(std::size_t buf_size, const sound_sample_t* in, std::complex<double>* out) {
			for (std::size_t i = 0; i < buf_size; ++i) {
				out[i] = std::complex<double>{
						(0.54 - 0.46 * cos(2 * bdm::math::pi * i / (buf_size - 1))) * in[i],
						0};
			}
		}
	}

	delay_estimation::delay_estimation(std::size_t) {
	}

	size_t delay_estimation::get_delay(std::size_t buf_size, const sound_sample_t* s1, const sound_sample_t* s2) {
		bdm::math::assert_pow(buf_size);

		using array_type = std::complex<double>[];
		auto x1 = std::make_unique<array_type>(buf_size);
		auto x1_fft = std::make_unique<array_type>(buf_size);
		auto x2 = std::make_unique<array_type>(buf_size);
		auto x2_fft = std::make_unique<array_type>(buf_size);
		auto y_fft = std::make_unique<array_type>(buf_size);
		auto y = std::make_unique<array_type>(buf_size);

		window(buf_size, s1, x1.get());
		window(buf_size, s2, x2.get());

		_fft.fft(buf_size, x1.get(), x1_fft.get());
		_fft.fft(buf_size, x2.get(), x2_fft.get());

		//make cross spectrum
		for (std::size_t i = 0; i < buf_size; i++) {
			y_fft[i] = std::conj(x1_fft[i]) * x2_fft[i];
		}

		_fft.ifft(buf_size, y_fft.get(), y.get());

		//estimate time shift
		double max = 0;
		std::size_t peek = 0;
		for (std::size_t i = 0; i < buf_size; i++) {
			if (y[i].real() > max) {
				peek = i;
				max = y[i].real();
			}
		}
		//printf( "time shift : %d\n", (peek + FRAMELEN/2)%FRAMELEN - FRAMELEN/2 );
		std::size_t delay = (peek + buf_size / 2) % buf_size - buf_size / 2;

		return delay;
	}
}
