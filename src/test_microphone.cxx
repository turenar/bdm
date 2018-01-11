#include <chrono>
#include <fstream>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdm/device/demultiplexer.hxx"
#include "bdm/device/mcp3004.hxx"
#include "bdm/device/gpio/gpio_wrapper.hxx"
#include "bdm/device/spi/spidev.hxx"
#include "bdm/task/sound_buffer.hxx"
#include "bdmbin/logger.hxx"

int main(int, char**) {
	bdmbin::logger logger;
	logger.install_logger();
	try {
		bdm::device::spi::config conf{};
		conf.bits_per_word = 8;
		conf.max_speed = 1000'0000;
		conf.spi_mode = bdm::device::spi::mode::DEFAULT;
		bdm::device::spi::spidev spi(bdm::unique_fd::open("/dev/spidev0.0", O_RDWR), conf);
		bdm::device::mcp3004 m(spi);
		bdm::task::sound_buffer buf(m);

		buf.fill();
		for (std::size_t ch = 0; ch < buf.microphone_count; ++ch) {
			auto b = buf.get_buf(ch);
			std::int8_t tmp[buf.buf_size];
			std::transform(b, b + buf.buf_size, tmp, [](std::int16_t s) { return static_cast<std::int8_t>(s); });
			std::ofstream os("ch" + std::to_string(ch) + ".raw");
			os.write(reinterpret_cast<const char*>(tmp), buf.buf_size);
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
