#include "bdm/device/spi/spidev.hxx"
#include "bdm/logger/logger.hxx"
#include "bdm/exception/bdm_system_exception.hxx"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

namespace bdm::device::spi {
	namespace {
		template<typename... T>
		void check_ioctl(unique_fd& fd, unsigned long int request, T ...args) {
			int ret = ioctl(fd.get(), request, std::forward<T>(args)...);
			if (ret == -1) {
				BDM_THROW_EXCEPTION(bdm::exception::bdm_system_exception(errno, std::system_category()));
			}
		}
	}

	spidev::spidev(unique_fd&& device_fd, const config& c) : _fd(std::move(device_fd)), _conf(c) {
		check_ioctl(_fd, SPI_IOC_RD_MODE, &c.spi_mode);
		check_ioctl(_fd, SPI_IOC_WR_MODE, &c.spi_mode);
		check_ioctl(_fd, SPI_IOC_RD_BITS_PER_WORD, &c.bits_per_word);
		check_ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &c.bits_per_word);
		check_ioctl(_fd, SPI_IOC_RD_MAX_SPEED_HZ, &c.max_speed);
		check_ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &c.max_speed);
	}

	void spidev::transfer(std::uint32_t size, const std::uint8_t* tx, std::uint8_t* rx) {
		spi_ioc_transfer tr = {};
		tr.tx_buf = reinterpret_cast<std::uint64_t>(tx);
		tr.rx_buf = reinterpret_cast<std::uint64_t>(rx);
		tr.len = size;
		tr.speed_hz = _conf.max_speed;
		tr.bits_per_word = _conf.bits_per_word;

		check_ioctl(_fd, SPI_IOC_MESSAGE(1), &tr);
	}
}
