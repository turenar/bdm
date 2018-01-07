#include "bdm/device/demultiplexer.hxx"
#include <cassert>

namespace bdm::device {
	demultiplexer::demultiplexer(gpio::gpio_wrapper& wrapper, int enabler, const int (& selector)[3]) : _gpio(wrapper) {
		_enabler_pin = static_cast<uint8_t>(enabler);
		_gpio.set_pin_mode(enabler, gpio::io_mode::output);
		for (int i = 0; i < 3; ++i) {
			_selector_pin[i] = static_cast<uint8_t>(selector[i]);
			_gpio.set_pin_mode(selector[i], gpio::io_mode::output);
		}
		enable();
	}

	void demultiplexer::select(int number) {
		assert(number >= 0 && number < 8);
		for (int i = 0; i < 3; ++i) {
			_gpio.write(_selector_pin[i], 1 == (number & (1 << i)));
		}
	}

	void demultiplexer::disable() { enable(false); }

	void demultiplexer::enable() { enable(true); }

	void demultiplexer::enable(bool enabled) {
		_gpio.write(_enabler_pin, enabled);
	}
}
