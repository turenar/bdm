#pragma once

#include <memory>
#include "bdm/general_definition.hxx"

namespace bdm::device {
	class mcp3004;
}

namespace bdm::task {
	class sound_buffer {
	public:
		static constexpr std::size_t microphone_count = 4;
		static constexpr std::size_t shift_width = 16 - 10;

		sound_buffer(bdm::device::mcp3004&, std::size_t buf_size);

		void fill();

		const sound_sample_t* get_buf(std::size_t index);

	private:
		bdm::device::mcp3004& _converter;
		const std::size_t _buf_size;
		std::array<std::unique_ptr<sound_sample_t[]>, microphone_count> _buf;
	};
}
