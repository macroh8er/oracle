#ifndef SRVLOG_H
#define SRVLOG_H

#include <vector>
#include <string>
#include <memory>
#include <boost/serialization/strong_typedef.hpp>

namespace oracle {

/*! \struct player_id
 *  \brief Describes a player reference
 *
 *  Detailed description
 */
struct player_id {
	std::string id;
	int team; /* 0 = radiant, 1 = dire */
	bool party; /* whether the player is party of the local player */
} /* optional variable list */;

BOOST_STRONG_TYPEDEF(std::string, log_path)
BOOST_STRONG_TYPEDEF(std::string, log_content)

/*! \class srvlog
 *  \brief Parse the server_log.txt file
 *
 *  Read the dota server_log.txt file for information
 */
class srvlog
{
public:
	srvlog(const log_path path);
	srvlog(const log_content content);
	srvlog();

	bool parse();
	std::vector<player_id> getPlayers();

	~srvlog();

protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */
};

} /* namespace oracle */

#endif /* end of include guard: SRVLOG_H */
