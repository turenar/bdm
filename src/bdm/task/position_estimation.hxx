#pragma once

#include <array>
#include "bdm/vector3d.hxx"
#include "bdm/general_definition.hxx"

namespace bdm::task {
	class position_estimation {
	public:
		vector3d estimate(const std::array<double, channels_count>& in);
	};
}
