#include <core/srvlog/srvlog.hpp>

#include <fstream>
#include <exception>
#include <iostream>
#include <regex>

/*
 * time / date
 */
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

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

	/*
	 * store player ids
	 */
	std::vector<player_id> player_ids;

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
	 * also build the date / time from the current time to make sure we don't
	 * detect any old games
	 */

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream test_regex_str;
	test_regex_str <<
		std::put_time(std::localtime(&in_time_t), "%m/%d/%Y - %H:%M:%S") <<
		": ([0-9]+).([0-9]+).([0-9]+).([0-9]+):([0-9]+) \\(Lobby ([0-9]+) ([A-Z_]+).+";

	std::regex test_regex(test_regex_str.str());

	if (!std::regex_search(last_line, test_regex)) {
		return false;
	}

	/*
	 * get all instances of user id
	 */
	std::regex search_regex("([0-9]:\\[U:[0-9]:([0-9]+)\\])");


	std::vector<std::string> ids;
	for(std::sregex_iterator it(last_line.begin(), last_line.end(), search_regex), end; it != end; it++) {
		std::smatch match = *it;
		/*
		 * id is [2]
		 */
		ids.push_back(match[2]);
	}

	/*
	 * if there is less than 10 ids, ignore this game
	 */
	if (ids.size() < 10) {
		return false;
	}

	for (int i = 0; i < 10; i++) {
		player_id addition { ids[i], (i >= 5), 0 };
		player_ids.push_back(addition);
	}

	for (int i = 10; i < ids.size(); i++) {
		for (int x = 0; x < 10; x++) {
			if(player_ids[x].id == ids[i]) {
				player_ids[x].party = true;
			}
		}
	}


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

std::vector<player_id> srvlog::getPlayers() {
	return m_impl->player_ids;
}

srvlog::~srvlog() = default;


} /* namespace oracle */

