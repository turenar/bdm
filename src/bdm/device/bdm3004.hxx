#pragma once

#include <cstdint>

namespace bdm::device {
	namespace spi {
		class spidev;
	}

	class bdm3004 {
	public:
		bdm3004(bdm::device::spi::spidev& dev);

		std::uint16_t read(std::uint8_t channel);
		std::uint8_t read_as_byte(std::uint8_t channel);

	private:
		static constexpr std::size_t packet_size = 3;
		static constexpr std::uint8_t single_bit = 0x80;

		static constexpr std::uint8_t compose_channel(std::uint8_t ch) {
			return single_bit | (ch << 4);
		}

		bdm::device::spi::spidev& _dev;
	};
}