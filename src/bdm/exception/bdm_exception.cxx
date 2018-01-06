#include "bdm/exception/bdm_exception.hxx"
#include <iostream>

namespace bdm::exception {
	void pabort(const std::string& message) {
		std::cerr << message << std::endl;
		abort();
	}
}