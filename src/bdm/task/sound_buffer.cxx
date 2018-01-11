#include "bdm/task/sound_buffer.hxx"
#include "bdm/math/util.hxx"
#include "bdm/device/mcp3004.hxx"

namespace bdm::task {

	sound_buffer::sound_buffer(bdm::device::mcp3004& converter, std::size_t buf_size)
			: _converter(converter), _buf_size(buf_size) {
		bdm::math::assert_pow(buf_size);
		for (auto& channel_buf:_buf) {
			channel_buf = std::make_unique<sound_sample_t[]>(buf_size);
		}
	}

	void sound_buffer::fill() {
		for (std::size_t i = 0; i < _buf_size; ++i) {
			for (std::uint8_t ch = 0; ch < _buf.size(); ++ch) {
				std::uint16_t unsigned_sample = _converter.read(ch) << shift_width;
				_buf[ch][i] = static_cast<unsigned short>(static_cast<int>(unsigned_sample) - 0x8000);
			}
		}
	}

	const sound_sample_t* sound_buffer::get_buf(std::size_t index) {
		return _buf[index].get();
	}
}
