#include <core/env/env.hpp>

#include <boost/program_options.hpp>
#include <iostream>

namespace oracle {

namespace po = boost::program_options;

struct env::impl
{
	std::unique_ptr<po::options_description> description;
	po::variables_map vm;

	impl(int* argc, char** argv);
	~impl() = default;
};

env::impl::impl(int* argc, char** argv) {

	/*
	 * build description
	 */
	description = std::make_unique<
		po::options_description>("Allowed options");

	po::options_description& desc = *description;

	desc.add_options()
		("help",	"Produce help message")
		("verbose",	"Enable verbose debugging logging")
		("silent",	"Disable all forms of logging")
		("log",		po::value<std::string>(), "Path of log file")
		("backends",	po::value<std::vector<std::string>>(), "List of data backends to use")
		("debug_ignore_time",	"core/srvlog ignores time validation")
		;

	/*
	 * if default params, ignore
	 */
	if (argc == nullptr || argv == nullptr) {
		return;
	}

	po::store(po::parse_command_line(*argc, argv, desc), vm);
	po::notify(vm);
}

/*
 * interface
 */

env::env()
	: m_impl(std::make_unique<impl>(nullptr, nullptr))
{
	
}

env& env::get() {
	static env environment;
	return environment;
}

void env::parse(int& argc, char** argv) {
	get().m_impl = std::make_unique<impl>(&argc, argv);
}

const void env::printOptions() {
	std::cout << *m_impl->description << std::endl;
}

/*
 *  environment
 */

const bool env::needsHelp() {
	return m_impl->vm.count("help");
}

const bool env::isVerbose() {
	return m_impl->vm.count("verbose");
}

const bool env::isSilent() {
	return m_impl->vm.count("silent");
}

const std::string env::logPath() {
	return m_impl->vm["log"].as<std::string>();
}

/*
 * degug environment
 */

const bool env::debug_shouldIgnoreTime() {
	return m_impl->vm.count("debug_ignore_time");
}

env::~env() = default;


} /* namespace oracle */
