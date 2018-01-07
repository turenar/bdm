#pragma once

#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <utility>
#include "bdm/exception/bdm_system_exception.hxx"

namespace bdm {
	class unique_fd {
	public:
		using fd_type = int;
		static constexpr fd_type invalid_fd = -1;

		static unique_fd open(const char* pathname, int flags);

		unique_fd();
		explicit unique_fd(int);
		unique_fd(const unique_fd&) = delete;
		unique_fd(unique_fd&&) noexcept;
		unique_fd& operator=(const unique_fd&) = delete;
		unique_fd& operator=(unique_fd&&) noexcept;

		~unique_fd();

		explicit operator fd_type();
		fd_type get();
		explicit operator bool() const;

	private:
		fd_type _fd;
	};

	inline unique_fd unique_fd::open(const char* pathname, int flags) {
		int fd = ::open(pathname, flags);
		if (fd == -1) {
			BDM_THROW_EXCEPTION(bdm::exception::bdm_system_exception(errno, std::system_category()));
		}
		return unique_fd(fd);
	}

	inline unique_fd::unique_fd() : _fd(invalid_fd) {}

	inline unique_fd::unique_fd(int fd) : _fd(fd) {}

	inline unique_fd::unique_fd(unique_fd&& fd) noexcept { *this = std::move(fd); }

	inline unique_fd& unique_fd::operator=(unique_fd&& another) noexcept {
		_fd = another._fd;
		another._fd = invalid_fd;
		return *this;
	}

	inline unique_fd::~unique_fd() {
		if (*this) {
			int result = close(_fd);
			(void) result;
		}
	}

	inline unique_fd::operator fd_type() { return _fd; }

	inline unique_fd::fd_type unique_fd::get() { return _fd; }

	inline unique_fd::operator bool() const { return _fd >= 0; }
}