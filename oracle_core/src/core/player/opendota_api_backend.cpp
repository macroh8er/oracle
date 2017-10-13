#include <core/player/opendota_api_backend.hpp>
#include <core/logging/logging.hpp>

#include <map>

namespace oracle {
namespace Impl {

/*
 * Mapping paths / api routes to the data that they provide
 * by doing this we can minimise network traffic while maximising data
 */

opendota_api_backend::opendota_api_backend(const player_id id)
	: backend(id)
{
	
}

/*
 * implement backend
 */

std::string opendota_api_backend::name() {
	return "opendota_api_backend";
}

std::future<fetch_t> opendota_api_backend::fetchData() {

	/*
	 * https://docs.opendota.com/#tag/players%2Fpaths%2F~1players~1%7Baccount_id%7D%2Fget
	 *
	 * https://api.opendota.com/api/players/{0}
	 * 	solo_mmr, party_mmr, name, avatar, mmr_estimate
	 */

	return std::async(std::launch::async, [this]() -> fetch_t {

	json data = backend::GETjson( fmt::format("https://api.opendota.com/api/players/{0}", m_id.id) );
	fetch_t fetched;

	if (data["mmr_estimate"].size() == 0) {
		/*
		 * Not valid, empty profile
		 * leave fetch_t fields default
		 */
		return fetched;
	}

	if (data["solo_competitive_rank"].is_null()) {
		fetched.solo_mmr = data["mmr_estimate"]["estimate"];
		fetched.solo_estimated = true;
	}
	else {
		fetched.solo_mmr = data["solo_competitive_rank"];
	}

	fetched.party_mmr
	= (data["competitve_rank"].is_null()) ? 0 : data["competitve_rank"].get<int>();

	fetched.country_code
	= (data["profile"]["loccountrycode"].is_null()) ? "" : data["profile"]["loccountrycode"].get<std::string>();

	fetched.name		= data["profile"]["personaname"];
	fetched.avatar		= data["profile"]["avatar"];

	return fetched;

	});

}

} /* namespace Impl */
} /* namespace oracle */


