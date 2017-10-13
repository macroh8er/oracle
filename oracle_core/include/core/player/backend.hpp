#ifndef BACKEND_H
#define BACKEND_H

#include <core/srvlog/srvlog.hpp>
#include <future>

#include <json.hpp>
using json = nlohmann::json;

namespace oracle {
namespace Impl {

/*! \class backend
 *  \brief Used for fetching information about players
 *
 *  backend is a base class that is used for an interface for implementing backends for getting
 *  information about the player.
 *
 *  To minimize network usage, backends should cache their results and are encouraged to store
 *  surplus data for future, unrelated calls.
 *
 *  For example, if a consumer calls fetchName() and the backend's API offers the player's name
 *  and solo mmr, name should be returned and solo mmr be stored for any future fetchSoloMMR()
 *  calls.
 */
class backend
{
public:
	backend(const player_id id);
	virtual ~backend() {}

	/*
	 * Backend name
	 */
	virtual std::string name() = 0;

	/*
	 * Fetch the player's display name
	 */
	virtual std::future<std::string> fetchName() = 0;

	/*
	 * Like core/player::getSoloMMR, estimated MMR is returned if the real MMR is undecidable
	 */
	virtual std::future<int> fetchSoloMMR() = 0;

	/*
	 * Fetches party MMR, if undecidable return 0
	 */
	virtual std::future<int> fetchPartyMMR() = 0;

	/*
	 * Country code
	 */
	virtual std::future<std::string> fetchCountryCode() = 0;

	/*
	 * Avatar
	 */
	virtual std::future<std::string> fetchAvatar() = 0;

	/*
	 * Used for backend implementations
	 * These functions are syncrhonous, they should be invoked with async if you want
	 * to return a future
	 */
	static std::string 	GET(const std::string url);
	static json		GETjson(const std::string url);

protected:
	const player_id m_id;

	/*
	 * Cached values
	 */
	std::string 	m_name;
	int		m_solo_mmr;
	int		m_party_mmr;

};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: BACKEND_H */
