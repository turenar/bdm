#pragma once

#include <cstdint>
#include "bdm/device/gpio/gpio_wrapper.hxx"

namespace bdm::device {
	class demultiplexer {
	public:
		demultiplexer(gpio::gpio_wrapper&, int enabler, const int (& selector)[3]);

		void select(int number);
		void disable();
		void enable();
		void enable(bool enabled);
	private:
		gpio::gpio_wrapper& _gpio;
		std::uint8_t _enabler_pin;
		std::uint8_t _selector_pin[3];
	};
}
