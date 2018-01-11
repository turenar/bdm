#include <chrono>
#include <iostream>
#include <fstream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdm/device/demultiplexer.hxx"
#include "bdm/device/gpio/gpio_wrapper.hxx"
#include "bdmbin/logger.hxx"
#include "bdm/task/sound_buffer.hxx"
#include "bdm/device/mcp3004.hxx"
#include "bdm/device/spi/spidev.hxx"
#include "bdm/task/delay_estimation.hxx"
#include "bdm/task/position_estimation.hxx"

int main(int, char**) {
	constexpr std::size_t FRAMELEN = 16384;
	constexpr double sample_time = 0.1;
	bdmbin::logger logger;
	logger.install_logger();
	try {
		FILE* fpDAT;

		if (NULL == (fpDAT = fopen("a.raw", "r"))) {
			exit(1);
		}
		auto sigdata1 = std::make_unique<short[]>(FRAMELEN);
		auto sigdata2 = std::make_unique<short[]>(FRAMELEN);
		auto sigdata3 = std::make_unique<short[]>(FRAMELEN);
		auto sigdata4 = std::make_unique<short[]>(FRAMELEN);
		fseek(fpDAT, 15 * sizeof(short), SEEK_SET);
		fread(sigdata1.get(), sizeof(short), FRAMELEN, fpDAT);
		fseek(fpDAT, 0 * sizeof(short), SEEK_SET);
		fread(sigdata2.get(), sizeof(short), FRAMELEN, fpDAT);
		fseek(fpDAT, 0 * sizeof(short), SEEK_SET);
		fread(sigdata3.get(), sizeof(short), FRAMELEN, fpDAT);
		fseek(fpDAT, 5 * sizeof(short), SEEK_SET);
		fread(sigdata4.get(), sizeof(short), FRAMELEN, fpDAT);
		fclose(fpDAT);
		bdm::task::delay_estimation de;
		double T1 = de.get_delay(FRAMELEN, sigdata1.get(), sigdata2.get()) * sample_time;
		double T2 = de.get_delay(FRAMELEN, sigdata1.get(), sigdata3.get()) * sample_time;
		double T3 = de.get_delay(FRAMELEN, sigdata1.get(), sigdata4.get()) * sample_time;

		bdm::task::position_estimation pe;
		double p[3];
		pe.estimate_position(0, T1, T2, T3, p);
			printf("%lf %lf %lf\n", p[0], p[1], p[2]);
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
