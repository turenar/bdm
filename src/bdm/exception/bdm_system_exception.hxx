#pragma once

#include "bdm/exception/bdm_exception.hxx"

namespace bdm::exception {
	struct bdm_system_exception : bdm_exception {
		bdm_system_exception(int ec, const std::error_category& cat)
				: bdm_exception(std::error_code(ec, cat).message()) {
		}

		bdm_system_exception(const std::string& prefix, int ec, const std::error_category& cat)
				: bdm_exception(prefix + ": " + std::error_code(ec, cat).message()) {
		}
	};
}
