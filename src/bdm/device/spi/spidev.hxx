#pragma once

#include <string>
#include "config.hxx"
#include "bdm/unique_fd.h"

namespace bdm::device::spi {
	class spidev {
	public:
		spidev(unique_fd&& device_fd, const config&);

		void transfer(std::uint32_t size, const std::uint8_t* rx, std::uint8_t* tx);

		template<std::size_t Size>
		void transfer(const std::uint8_t(& rx)[Size], std::uint8_t(& tx)[Size]) {
			transfer(static_cast<uint32_t>(Size), rx, tx);
		}

	protected:
		unique_fd _fd;
		config _conf;
	};
}