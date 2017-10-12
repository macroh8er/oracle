#ifndef BACKEND_H
#define BACKEND_H

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
	backend();
	virtual ~backend();

	/*
	 * Fetch the player's display name
	 */
	virtual const std::string fetchName();

	/*
	 * Like core/player::getSoloMMR, estimated MMR is returned if the real MMR is undecidable
	 */
	virtual const int fetchSoloMMR();

	/*
	 * Returns party MMR, if undecidable return 0
	 */
	virtual const int fetchPartyMMR();

};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: BACKEND_H */
