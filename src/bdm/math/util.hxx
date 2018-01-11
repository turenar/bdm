#pragma once

#include <cassert>
#include <cstddef>

namespace bdm::math {
	constexpr double pi = 3.14159265359;

	constexpr bool is_pow(std::size_t s) {
		return !(s & (s - 1));
	}

	inline void assert_pow(std::size_t s) {
		assert(is_pow(s));
	}
}
