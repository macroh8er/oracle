#ifndef BACKEND_H
#define BACKEND_H

#include <core/srvlog/srvlog.hpp>
#include <future>

#include <json.hpp>
using json = nlohmann::json;

namespace oracle {
namespace Impl {

/*! \struct fetch_t
 *  \brief Data which is returned when fetching
 *
 *  Detailed description
 */
struct fetch_t {
	std::string name;
	int solo_mmr;
	bool solo_estimated;
	int party_mmr;
	std::string country_code;
	std::string avatar;

	fetch_t()
		: name(""), solo_mmr(0), solo_estimated(false), party_mmr(0), country_code(""), avatar("")
	{}
};

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
	 * Asyncrhonously fetch player data
	 */
	virtual std::future<fetch_t> fetchData() = 0;

	/*
	 * Used for backend implementations
	 * These functions are syncrhonous, they should be invoked with async if you want
	 * to return a future
	 */
	static std::string 	GET(const std::string url);
	static json		GETjson(const std::string url);

protected:
	const player_id m_id;
};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: BACKEND_H */
