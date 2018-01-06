#pragma once

#include <g3log/g3log.hpp>
#include "log_levels.hxx"

#ifdef NDEBUG
struct empty_stream {
};

template<typename T>
inline empty_stream& operator<<(empty_stream& st, T&) {
	return st;
}

#undef LOG
#define LOG(level) (empty_stream())
#undef LOG_IF
#define LOG_IF(level, boolean_expression) LOG(level)
#undef CHECK
#define CHECK(boolean_expression) (empty_stream())
#undef LOGF
#define LOGF(level, printf_like_message, ...) LOG(level)
#undef LOGF_IF
#define LOGF_IF(level, boolean_expression, printf_like_message, ...) LOG(level)
#undef CHECKF
#define CHECKF(boolean_expression, printf_like_message, ...) CHECK(boolean_expression)
#undef CHECK_F
#endif