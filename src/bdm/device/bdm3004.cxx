#include "bdm/device/bdm3004.hxx"
#include <cassert>
#include "bdm/device/spi/spidev.hxx"

namespace bdm::device {
	bdm3004::bdm3004(bdm::device::spi::spidev& dev) : _dev(dev) {
	}

	std::uint16_t bdm3004::read(std::uint8_t channel) {
		assert(channel < 4);
		const std::uint8_t tx[/*packet_size*/] = {0x01, compose_channel(channel), 0x00};
		std::uint8_t rx[packet_size];
		_dev.transfer(tx, rx);
		return static_cast<uint16_t>(((tx[1] & 0x03) << 8) | tx[2]);
	}

	std::uint8_t bdm3004::read_as_byte(std::uint8_t channel) {
		return static_cast<std::uint8_t>(read(channel) >> 2);
	}
}