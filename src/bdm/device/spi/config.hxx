#pragma once

#include "mode.hxx"

namespace bdm::device::spi {
	struct config {
		std::uint32_t max_speed;
		mode spi_mode;
		std::uint8_t bits_per_word;
	};
}