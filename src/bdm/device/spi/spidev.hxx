#pragma once

#include <string>
#include "config.hxx"
#include "bdm/unique_fd.h"

namespace bdm::device::spi {
	class spidev {
	public:
		spidev(unique_fd&& device_fd, const config&);

		void transfer(std::uint32_t size, const std::uint8_t* tx, std::uint8_t* rx);

		template<std::size_t Size>
		void transfer(const std::uint8_t(& tx)[Size], std::uint8_t(& rx)[Size]) {
			transfer(static_cast<uint32_t>(Size), tx, rx);
		}

	protected:
		unique_fd _fd;
		config _conf;
	};
}