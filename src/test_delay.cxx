#include <chrono>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <g3log/logworker.hpp>
#include "bdm/device/demultiplexer.hxx"
#include "bdm/device/gpio/gpio_wrapper.hxx"
#include "bdm/task/delay_estimation.hxx"
#include "bdm/task/position_estimation.hxx"
#include "bdmbin/logger.hxx"

int main(int, char**) {
	constexpr std::size_t frame_len = 4096;
	constexpr double sample_time = 1. / 5000;
	using bdm::sound_sample_t;

	bdmbin::logger logger;
	logger.install_logger();
	try {
		FILE* fpDAT;
		if (NULL == (fpDAT = fopen("a.raw", "r"))) {
			exit(1);
		}

		auto sigdata1 = (sound_sample_t*) calloc(sizeof(sound_sample_t), frame_len);
		auto sigdata2 = (sound_sample_t*) calloc(sizeof(sound_sample_t), frame_len);
		auto sigdata3 = (sound_sample_t*) calloc(sizeof(sound_sample_t), frame_len);
		auto sigdata4 = (sound_sample_t*) calloc(sizeof(sound_sample_t), frame_len);
		fseek(fpDAT, 15 * sizeof(sound_sample_t), SEEK_SET);
		fread(sigdata1, sizeof(sound_sample_t), frame_len, fpDAT);
		fseek(fpDAT, 0 * sizeof(sound_sample_t), SEEK_SET);
		fread(sigdata2, sizeof(sound_sample_t), frame_len, fpDAT);
		fseek(fpDAT, 0 * sizeof(sound_sample_t), SEEK_SET);
		fread(sigdata3, sizeof(sound_sample_t), frame_len, fpDAT);
		fseek(fpDAT, 5 * sizeof(sound_sample_t), SEEK_SET);
		fread(sigdata4, sizeof(sound_sample_t), frame_len, fpDAT);
		fclose(fpDAT);
		//preparation end
		bdm::task::delay_estimation de(frame_len);
		double T1, T2, T3;

		T1 = de.get_delay(frame_len, sigdata1, sigdata2) * sample_time;
		T2 = de.get_delay(frame_len, sigdata1, sigdata2) * sample_time;
		T3 = de.get_delay(frame_len, sigdata1, sigdata2) * sample_time;

		bdm::task::position_estimation pe;
		auto p = pe.estimate({0, T1, T2, T3});
		printf("%lf %lf %lf\n", p.x, p.y, p.z);
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
