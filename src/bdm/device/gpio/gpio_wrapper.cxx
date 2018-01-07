#include "bdm/device/gpio/gpio_wrapper.hxx"
#include <wiringPi.h>
#include "bdm/exception/bdm_exception.hxx"

namespace bdm::device::gpio {
	gpio_wrapper::gpio_wrapper() {
		if (wiringPiSetupGpio() == -1) {
			BDM_THROW_EXCEPTION(bdm::exception::bdm_exception("wiringPiSetupGpio failed"));
		}
	}

	void gpio_wrapper::set_pin_mode(int pin, io_mode m) {
		pinMode(pin, static_cast<int>(m));
	}

	void gpio_wrapper::write(int pin, bool high) {
		digitalWrite(pin, high);
	}
}