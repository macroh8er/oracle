#include <core/logging/logging.hpp>

namespace oracle {

struct logging::impl
{

	/*
	 * for combining sinks
	 */
	std::vector<spdlog::sink_ptr> sinks;
	spdlog::sink_ptr stdout_sink;
	spdlog::sink_ptr file_sink;

	std::shared_ptr<spdlog::logger> sink;

	impl(const std::string log_path);
	~impl() = default;
};

logging::impl::impl(const std::string log_path) {
	/*
	 * setup sinks
	 */
	stdout_sink = spdlog::sinks::stdout_sink_mt::instance();
	file_sink = std::make_shared<spdlog::sinks::simple_file_sink_mt>(log_path);

	sinks.push_back(stdout_sink);
	sinks.push_back(file_sink);
	
	sink = std::make_shared<spdlog::logger>("oracle", begin(sinks), end(sinks));
	spdlog::register_logger(sink);
}

/*
 * interface
 */

logging::logging()
	: m_impl(std::make_unique<impl>("oracle_log.txt"))
{
	
}

spdlog::logger& logging::get() {
	static logging logger;
	return *logger.m_impl->sink.get();
}

logging& logging::configure() {
	return *this;
}

void logging::setPath(const std::string log_path) {
	m_impl = std::make_unique<impl>(log_path);
}


logging::~logging() = default;

} /* namespace oracle */

