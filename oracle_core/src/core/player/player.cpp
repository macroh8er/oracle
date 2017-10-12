#include <core/player/player.hpp>

/*
 * TODO: add caching
 */

namespace oracle {

struct player::impl
{
	impl(const player_id id);
	~impl() = default;
};

player::impl::impl(const player_id id) {

}

/*
 * interface
 */

player::player(const player_id id)
	: m_impl(std::make_unique<impl>(id))
{
	
}

player::~player() = default;

} /* namespace oracle */

