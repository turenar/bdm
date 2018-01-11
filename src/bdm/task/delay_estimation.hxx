#pragma once
#include <cstdint>

namespace bdm::task {
	class delay_estimation {
	public:
		int get_delay(std::size_t buf_size, const std::int16_t* sig1, const std::int16_t* sig2);
	};
}
