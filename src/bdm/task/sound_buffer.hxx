#pragma once

#include <memory>

namespace bdm::device {
	class mcp3004;
}

namespace bdm::task {
	class sound_buffer {
	public:
		static constexpr std::size_t microphone_count = 4;
		static constexpr std::size_t buf_size = 65536;

		sound_buffer(bdm::device::mcp3004&);

		void fill();

		const std::uint8_t* get_buf(std::size_t index);

	private:
		bdm::device::mcp3004& _converter;
		std::array<std::unique_ptr<std::uint8_t[]>, microphone_count> _buf;
	};
}
