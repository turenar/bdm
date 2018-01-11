#pragma once

#include "bdm/device/gpio/io_mode.hxx"

namespace bdm::device::gpio{
	class gpio_wrapper{
	public:
		gpio_wrapper();

		void set_pin_mode(std::uint8_t pin, io_mode);
		void write(std::uint8_t pin, bool high);
		bool read(std::uint8_t pin);
	};
}
