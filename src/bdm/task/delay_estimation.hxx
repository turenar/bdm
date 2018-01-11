#pragma once

#include "bdm/general_definition.hxx"
#include "bdm/math/fast_fourier_transform.hxx"

namespace bdm::task {
	class delay_estimation {
	public:
		delay_estimation(std::size_t buf_size);

		std::size_t get_delay(std::size_t buf_size, const sound_sample_t*, const sound_sample_t*);

	private:
		bdm::math::fast_fourier_transform _fft;
	};
}
