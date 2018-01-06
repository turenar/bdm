#pragma once

#include "bdm/logger/logger.hxx"
#include <memory>

namespace g3 { class LogWorker; }

namespace bdmbin {
	class logger {
	public:
		logger();
		~logger();
		void install_logger();

	private:
		std::unique_ptr<g3::LogWorker> _worker;
	};
}
