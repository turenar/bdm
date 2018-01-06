#include "bdm/logger/logger.hxx"
#include "bdmbin/logger.hxx"
#include <g3log/logworker.hpp>
#include "version.hxx"
#include "bdm/logger/log_console_sink.hxx"

namespace bdmbin {
	logger::logger() = default;
	logger::~logger() = default;

	void logger::install_logger() {
#ifdef NDEBUG
		g3::only_change_at_initialization::setLogLevel(DEBUG, false);
#endif
		g3::only_change_at_initialization::setLogLevel(ERROR, true);
		auto worker = g3::LogWorker::createLogWorker();
//#ifdef _DEBUG
		worker->addSink(std::make_unique<bdm::logger::log_console_sink>(), &bdm::logger::log_console_sink::log);
//#endif
		g3::initializeLogging(worker.get());
		LOG(INFO) << "Version: " << bdm::VERSION;
		_worker = std::move(worker);
	}
}
