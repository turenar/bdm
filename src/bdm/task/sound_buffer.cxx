#include "bdm/task/sound_buffer.hxx"
#include <cassert>
#include "bdm/device/mcp3004.hxx"

namespace bdm::task {
	namespace {
		constexpr int threshold = 0x1000;

		bool is_pow(std::size_t s) {
			return !(s & (s - 1));
		}
	}

	sound_buffer::sound_buffer(bdm::device::mcp3004& converter)
			: _converter(converter) {
		assert(is_pow(buf_size));
		for (auto& channel_buf:_buf) {
			channel_buf = std::make_unique<std::int16_t[]>(buf_size);
		}
	}


	void sound_buffer::fill() {
		for (std::size_t i = 0; i < buf_size; ++i) {
			for (std::uint8_t ch = 0; ch < _buf.size(); ++ch) {
				std::int16_t val = _converter.read_as_signed(ch);
				_buf[ch][i] = static_cast<std::int16_t>(val < threshold ? 0 : val);
			}
		}
	}

	const std::int16_t* sound_buffer::get_buf(std::size_t index) {
		return _buf[index].get();
	}
}
