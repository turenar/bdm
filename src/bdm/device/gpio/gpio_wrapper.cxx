#include "bdm/device/gpio/gpio_wrapper.hxx"
#include <wiringPi.h>
#include "bdm/exception/bdm_exception.hxx"
#include "bdm/logger/logger.hxx"

namespace bdm::device::gpio {
	gpio_wrapper::gpio_wrapper() {
		if (wiringPiSetupGpio() == -1) {
			BDM_THROW_EXCEPTION(bdm::exception::bdm_exception("wiringPiSetupGpio failed"));
		}
	}

	void gpio_wrapper::set_pin_mode(std::uint8_t pin, io_mode m) {
		LOG(DEBUG) << "gpio.set_pin_mode: pin=" << static_cast<int>(pin) << ", mode=" << static_cast<int>(m);
		pinMode(pin, static_cast<int>(m));
	}

	void gpio_wrapper::write(std::uint8_t pin, bool high) {
		LOG(DEBUG) << "gpio.write: pin=" << static_cast<int>(pin) << ", high=" << std::boolalpha << high;
		digitalWrite(pin, high);
	}

	bool gpio_wrapper::read(std::uint8_t pin) {
		int val = digitalRead(pin);
		LOG(DEBUG) << "gpio.read: pin=" << static_cast<int>(pin) << ", high=" << val;
		return val != 0;
	}
}
