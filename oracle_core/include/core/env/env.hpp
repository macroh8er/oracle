#ifndef ENV_H
#define ENV_H

#include <string>
#include <memory>

namespace oracle {

/*! \class env
 *  \brief Holds config and cli information
 *
 *  Parses information from command line arguments and config files
 */
class env
{
public:

	static env& get();

	/*
	 * must be called beforehand
	 */
	static void parse(int& argc, char** argv);

	/*
	 * usable before parse
	 */
	const void printOptions();

	/*
	 * Whether the user needs help
	 */
	const bool needsHelp();

	/*
	 * Whether debugging logging messages should be enabled
	 */
	const bool isVerbose();

	/*
	 * Whether all logging should be disabled
	 */
	const bool isSilent();

	/*
	 * The relative path to the local log file
	 */
	const std::string logPath();

	/*
	 * A list of strings which represents the data backends to use
	 * 	opendota_api	- Opendota public api, usable for 90 requests per minute
	 * 	opendota_scrape	- Opendota html pages scraped
	 *
	 * Generally speaking the *_scrape backends are only used out of necessity and are only
	 * used when the *_api backends are unable to be used
	 */
	const std::string dataBackends();

	/*
	 * debugging options
	 */

	/*
	 * core/srvlog usually ignores any game lines which don't match in times. Disabling this
	 * feature allows easier debugging of obtaining backend information
	 */
	const bool debug_shouldIgnoreTime();


protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */

	/*
	 * singleton
	 */
	env();
	~env();
};

} /* namespace oracle */

#endif /* end of include guard: ENV_H */
