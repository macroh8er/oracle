#ifndef LOGGING_H
#define LOGGING_H

/*
 *  opt/spdlog
 */
#include <spdlog/spdlog.h>

namespace oracle {

/*! \class logging
 *  \brief Interface for spdlog sink
 *
 *  Logging class that sinks to oracle_log.txt and stdout
 */
class logging
{
public:
	void setPath(const std::string log_path);
	logging& configure();

	static spdlog::logger& get();

protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */

	/*
	 * singleton
	 */
	logging();
	~logging();

};

} /* namespace oracle */

#endif /* end of include guard: LOGGING_H */
