#include "bdm/exception/error_info.hxx"
#include <cstdlib>
#include <ostream>
#include "bdm/exception/bdm_exception.hxx"

extern "C" {
#include <execinfo.h>
}

namespace bdm {
	namespace internal {
		std::ostream& backtrace_info(std::ostream& os) {
			static const int MAX = 256;
			void* frames[MAX];
			int size = ::backtrace(frames, MAX);
			if (size <= 0 || size >= MAX) {
				return os << "backtrace failed (" << size << ")" << std::endl;
			}

			char** symbols = ::backtrace_symbols(frames, MAX);
			if (!symbols) {
				return os << "backtrace_symbols failed" << std::endl;
			}

			// skip error_info constructor
			for (int i = 2; i < size; ++i) {
				os << symbols[i] << std::endl;
			}

			::free(symbols);
			return os;
		}
	}
}
namespace boost {
	error_info<::bdm::exception::internal::backtrace_tag, ::bdm::exception::internal::backtrace_tag>::error_info() {
		std::ostringstream oss;
		oss << "---- backtrace begin ----\n"
		    << bdm::internal::backtrace_info
		    << "---- backtrace end ----\n";
		_s = oss.str();
	}
}
