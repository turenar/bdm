#include <chrono>
#include <cmath>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdm/device/demultiplexer.hxx"
#include "bdm/device/mcp3004.hxx"
#include "bdm/device/gpio/gpio_wrapper.hxx"
#include "bdm/device/spi/spidev.hxx"
#include "bdm/task/delay_estimation.hxx"
#include "bdm/task/position_estimation.hxx"
#include "bdm/task/sound_buffer.hxx"
#include "bdmbin/logger.hxx"

namespace {
	constexpr std::uint8_t gpio_button = 14;
	constexpr std::uint8_t gpio_demux_enabler = 23;
	constexpr std::uint8_t gpio_demux_selector[] = {17, 21, 22};

	constexpr bdm::device::spi::config get_spi_conf() {
		bdm::device::spi::config spi_conf{};
		spi_conf.bits_per_word = 8;
		spi_conf.max_speed = 1000'0000;
		spi_conf.spi_mode = bdm::device::spi::mode::DEFAULT;
		return spi_conf;
	}

	class runner {
	public:
		runner();

		void loop();
	private:
		bdm::device::gpio::gpio_wrapper _g;
		bdm::device::demultiplexer _demux;
		bdm::device::spi::spidev _sp;
		bdm::device::mcp3004 _mc;
		bdm::task::sound_buffer _buf;
		bdm::task::delay_estimation _de;
		bdm::task::position_estimation _pe;
	};

	runner::runner()
			: _demux(_g, gpio_demux_enabler, gpio_demux_selector),
			  _sp(bdm::unique_fd::open("/dev/spidev0.0", O_RDWR), get_spi_conf()),
			  _mc(_sp),
			  _buf(_mc) {
		_g.set_pin_mode(gpio_button, bdm::device::gpio::io_mode::input);
	}

	void runner::loop() {
		if (!_g.read(gpio_button)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
			return;
		}
		LOG(INFO) << "loop start";
		auto start = std::chrono::system_clock::now();
		_buf.fill();
		auto end = std::chrono::system_clock::now();

		using sec_type = std::chrono::duration<double, std::chrono::milliseconds::period>;
		auto sample_time = sec_type(end - start).count() / _buf.buf_size;

		LOG(DEBUG) << "sample time: " << sample_time;
		double t1 = _de.get_delay(_buf.buf_size, _buf.get_buf(0), _buf.get_buf(1)) * sample_time;
		double t2 = _de.get_delay(_buf.buf_size, _buf.get_buf(0), _buf.get_buf(2)) * sample_time;
		double t3 = _de.get_delay(_buf.buf_size, _buf.get_buf(0), _buf.get_buf(3)) * sample_time;
		LOG(INFO) << "estimated delay: " << t1 << ' ' << t2 << ' ' << t3;

		bdm::task::position_estimation pe;
		double p[3];
		pe.estimate_position(0, t1, t2, t3, p);
		LOG(INFO) << "estimated position: " << p[0] << ' ' << p[1] << ' ' << p[2];

		double angle;
		if (p[0] <= 0) {
			angle = 180 - std::atan((-p[1]) / p[0]);
		} else {
			angle = std::atan(p[1] / p[0]);
		}
		LOG(INFO) << "angle: " << angle;
	}
}

int main(int, char**) {
	bdmbin::logger logger;
	logger.install_logger();
	try {
		runner r;
		while (true) {
			r.loop();
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
