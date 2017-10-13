#include <core/player/player.hpp>
#include <core/player/opendota_api_backend.hpp>

/*
 * TODO: add caching
 */

namespace oracle {

struct player::impl
{
	Impl::opendota_api_backend backend;
	std::future<Impl::fetch_t> data;
	Impl::fetch_t fetched_data;

	impl(const player_id id);
	~impl() = default;

	const player_id id;
};

player::impl::impl(const player_id _id)
	: backend(_id), id(_id)
{
	data = backend.fetchData();
}


/*
 * interface
 */

player::player(const player_id id)
	: m_impl(std::make_unique<impl>(id))
{
	
}

void player::waitForData() {
	m_impl->fetched_data = m_impl->data.get();
}

const std::string player::id() {
	return m_impl->id.id;
}

const std::string player::getName() {
	return m_impl->fetched_data.name;
}

const int player::getSoloMMR() {
	return m_impl->fetched_data.solo_mmr;
}

const bool player::isSoloEstimated() {
	return m_impl->fetched_data.solo_estimated;
}

const int player::getPartyMMR() {
	return m_impl->fetched_data.party_mmr;
}

player::~player() = default;

} /* namespace oracle */

