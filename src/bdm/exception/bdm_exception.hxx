#pragma once

#include <exception>
#include <string>
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>
#include <boost/exception/detail/error_info_impl.hpp>
#include "bdm/exception/error_info.hxx"

namespace bdm::exception {
	class bdm_exception : public ::std::exception, public ::boost::exception {
	public:
		explicit bdm_exception(std::string s) : _what(std::move(s)) {}

		virtual const char* what() const noexcept override final { return _what.c_str(); }

	private:
		std::string _what;
	};

	[[noreturn]]
	void pabort(const std::string& message);
}

#ifdef NDEBUG
#define BDM_THROW_EXCEPTION(ex) pabort(ex.what())
#else
#define BDM_THROW_EXCEPTION(ex) BOOST_THROW_EXCEPTION((ex) << ::bdm::exception::errinfo_ipp_backtrace())
#endif
