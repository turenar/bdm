#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdmbin/logger.hxx"
#include "bdm/device/spi/spidev.hxx"
#include "bdm/device/mcp3004.hxx"

int main(int, char**) {
	bdmbin::logger logger;
	logger.install_logger();
	try {
		bdm::device::spi::config conf{};
		conf.bits_per_word = 8;
		conf.max_speed = 1000'0000;
		conf.spi_mode = bdm::device::spi::mode::DEFAULT;

		bdm::device::spi::spidev dev(bdm::unique_fd::open("/dev/spidev0.0", O_RDWR), conf);
		bdm::device::mcp3004 ad_conv(dev);
		for (int i = 0;i < 1024;++i) {
			std::cout << std::hex << static_cast<std::uint32_t>(ad_conv.read_as_byte(0)) << '\t'
					  << std::hex << static_cast<std::uint32_t>(ad_conv.read_as_byte(1)) << '\t'
					  << std::hex << static_cast<std::uint32_t>(ad_conv.read_as_byte(2)) << '\t'
					  << std::hex << static_cast<std::uint32_t>(ad_conv.read_as_byte(3)) << std::endl;
		}
	} catch (boost::exception& ex) {
		LOG(ERROR) << "uncaught exception";
		std::exception* stdex = dynamic_cast<std::exception*>(&ex);
		if (stdex) {
			LOG(ERROR) << "what: " << stdex->what();
		}
		LOG(ERROR) << boost::diagnostic_information_what(ex);
		return 1;
	} catch (std::exception& ex) {
		LOG(ERROR) << "uncaught exception";
		LOG(ERROR) << "what: " << ex.what();
		return 1;
	}
}
