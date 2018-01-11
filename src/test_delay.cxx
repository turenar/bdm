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

		bdm::task::delay_estimation de;
		double T1 = de.get_delay(buf.buf_size, buf.get_buf(0), buf.get_buf(1));
		double T1 = de.get_delay(buf.buf_size, buf.get_buf(0), buf.get_buf(1));
			sigdata1 = (short*)calloc( sizeof(short), FRAMELEN );
			sigdata2 = (short*)calloc( sizeof(short), FRAMELEN );
			sigdata3 = (short*)calloc( sizeof(short), FRAMELEN );
			sigdata4 = (short*)calloc( sizeof(short), FRAMELEN );
			fseek( fpDAT, 15*sizeof(short), SEEK_SET );
			fread( sigdata1, sizeof(short), FRAMELEN, fpDAT );
			fseek( fpDAT, 0*sizeof(short), SEEK_SET );
			fread( sigdata2, sizeof(short), FRAMELEN, fpDAT );
			fseek( fpDAT, 0*sizeof(short), SEEK_SET );
			fread( sigdata3, sizeof(short), FRAMELEN, fpDAT );
			fseek( fpDAT, 5*sizeof(short), SEEK_SET );
			fread( sigdata4, sizeof(short), FRAMELEN, fpDAT );
			fclose( fpDAT );
			//preparation end
			double T1, T2, T3;
			T1 = getDelay(sigdata1, sigdata2) * SAMPLETIME;
			T2 = getDelay(sigdata1, sigdata3) * SAMPLETIME;
			T3 = getDelay(sigdata1, sigdata4) * SAMPLETIME;
			double *p = (double*)malloc(sizeof(double) * 3);
			estimatePosition(0, T1, T2, T3, p);
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
