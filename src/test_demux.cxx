#include <chrono>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdm/device/gpio/gpio_wrapper.hxx"
#include "bdmbin/logger.hxx"

int main(int, char**) {
	bdmbin::logger logger;
	logger.install_logger();
	try {
		bdm::device::gpio::gpio_wrapper g;
		g.set_pin_mode(17, bdm::device::gpio::io_mode::output);
		g.set_pin_mode(21, bdm::device::gpio::io_mode::output);
		g.set_pin_mode(22, bdm::device::gpio::io_mode::output);
		while (true) {
			g.write(17, true);
			g.write(21, false);
			g.write(22, true);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			g.write(22, false);
			std::this_thread::sleep_for(std::chrono::seconds(1));
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
