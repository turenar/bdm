#pragma once

namespace bdm::device::gpio {
	enum class io_mode {
		input = 0,
		output = 1,
		pwm_output = 2,
	};
}