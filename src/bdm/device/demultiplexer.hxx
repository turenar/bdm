#pragma once

#include <cstdint>

namespace bdm::device {
	namespace gpio {
		class gpio_wrapper;
	}

	class demultiplexer {
	public:
		demultiplexer(gpio::gpio_wrapper&, std::uint8_t enabler, const std::uint8_t (& selector)[3]);

		void select(std::uint8_t number);
		void disable();
		void enable();
		void enable(bool enabled);

	private:
		gpio::gpio_wrapper& _gpio;
		std::uint8_t _enabler_pin;
		std::uint8_t _selector_pin[3];
	};
}
