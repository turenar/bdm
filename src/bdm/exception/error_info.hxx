#pragma once

#include <boost/exception/detail/error_info_impl.hpp>

namespace bdm::exception::internal {
	struct backtrace_tag {
	};

	std::ostream& backtrace_info(std::ostream& os);
}

namespace boost {
	template<>
	class error_info<::bdm::exception::internal::backtrace_tag, ::bdm::exception::internal::backtrace_tag> final
			: public boost::exception_detail::error_info_base {
	public:
		error_info();

		virtual std::string name_value_string() const override { return _s; }

	private:
		std::string _s;
	};
}
namespace bdm::exception {
	using errinfo_ipp_backtrace = ::boost::error_info<::bdm::exception::internal::backtrace_tag, ::bdm::exception::internal::backtrace_tag>;
}
