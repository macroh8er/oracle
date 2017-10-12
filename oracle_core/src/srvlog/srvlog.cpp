#include <core/srvlog/srvlog.hpp>

#include <fstream>
#include <exception>
#include <iostream>
#include <regex>

namespace oracle {

struct srvlog::impl
{
	/*
	 * for implementing srvlog::reprarse
	 */
	struct construction_ {
		const std::string arg;
		const int arg_type; /* 0 = content, 1 = path */

		construction_(const std::string _arg, const int _arg_type)
			: arg(_arg), arg_type(_arg_type)
			 {}
	};

	construction_ construction;

	impl(const log_content content);
	impl(const log_path path);

	bool parse(const std::string cnt);

	static std::string get_content(const log_path path);
	
	~impl() {}
};

std::string srvlog::impl::get_content(const log_path path) {

	std::ifstream file(path);
	if (!file) {
		throw std::runtime_error("Failed to load file");
	}

	std::string cnt( (std::istreambuf_iterator<char>(file) ),
			 (std::istreambuf_iterator<char>() )
		       );

	file.close();

	return cnt;
}

bool srvlog::impl::parse(std::string cnt) {

	/*
	 * extract regex
	 * 	([0-9]:\[U:[0-9]:([0-9]+)\])
	 *
	 * testing regex
	 * 	[0-9]+\/[0-9]+\/[0-9]+ - [0-9]+:[0-9]+:[0-9]+: [0-9]+.[0-9]+.[0-9]+.[0-9]+:[0-9]+ \(Lobby [0-9]+ ([A-Z_]+)
	 *
	 * testing regex matches the beginning of a game
	 */

	/*
	 * if ending character is a newline, ignore it
	 */

	if (cnt[cnt.length()-1] == '\n') {
		cnt.pop_back();
	}

	/*
	 * extract the last line of the file
	 */

	std::size_t end = cnt.length()-1;
	auto start = cnt.find_last_of('\n');

	if (start == std::string::npos) {
		start = 0;
	}

	std::string last_line = cnt.substr(start, end);

	/*
	 * test if the line is a vaild game beginning (aka not a loopback line)
	 */

	std::regex test_regex("([0-9]+)\\/([0-9]+)\\/([0-9]+) - ([0-9]+):([0-9]+):([0-9]+): ([0-9]+).([0-9]+).([0-9]+).([0-9]+):([0-9]+) \\(Lobby ([0-9]+) ([A-Z_]+).+");

	if (!std::regex_search(last_line, test_regex)) {
		return false;
	}

	/*
	 * get all instances of user id
	 */

	return true;
}


srvlog::impl::impl(const log_content content)
	: construction (content, 0)
{

}

srvlog::impl::impl(const log_path path)
	: construction (path, 1)
{

}


srvlog::srvlog(const log_path path)
	: m_impl(std::make_unique<impl>(path))
{

}

srvlog::srvlog(const log_content content)
	: m_impl(std::make_unique<impl>(content))
{
	
}

bool srvlog::parse() {

	/*
	 * if the construction was made with a path
	 */
	if (m_impl->construction.arg_type) {
		m_impl = std::make_unique<impl>(log_path(m_impl->construction.arg));
		return m_impl->parse( impl::get_content(log_path(m_impl->construction.arg)) );
	}

	return m_impl->parse(m_impl->construction.arg);
}

srvlog::~srvlog() = default;


} /* namespace oracle */

