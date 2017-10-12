#ifndef PLAYER_H
#define PLAYER_H

#include <core/srvlog/srvlog.hpp>

#include <string>
#include <memoery>

namespace oracle {

/*! \class player
 *  \brief Interface for storing and fetching player data
 *
 *  Caches earlier requests to minimize netork usage. Backends used depend
 *  on env::dataBackends and previous network utilisation
 */
class player
{
public:
	player(const player_id id);
	~player();

	/*
	 * Returns player profile name
	 */
	const std::string getName();

	/*
	 * Returns ranked solo MMR if applicable
	 * If this is undecidable, return estimated MMR
	 */
	const int getSoloMMR();

	/*
	 * Returns ranked party MMR if applicable
	 * If this is undecidable, returns 0
	 */
	const int getPartyMMR();



protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */
};

} /* namespace oracle */

#endif /* end of include guard: PLAYER_H */
