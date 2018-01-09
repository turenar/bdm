#pragma once

#include "bdm/device/gpio/io_mode.hxx"

namespace bdm::device::gpio{
	class gpio_wrapper{
	public:
		gpio_wrapper();

		void set_pin_mode(int pin, io_mode);
		void write(int pin, bool high);
		bool read(int pin);
	};
}
