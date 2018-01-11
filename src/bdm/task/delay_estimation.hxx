#pragma once
#include <cstdint>

namespace bdm::task {
	class delay_estimation {
	public:
		int get_delay(std::size_t buf_size, std::int16_t* sig1, std::int16_t* sig2);
	};
}
