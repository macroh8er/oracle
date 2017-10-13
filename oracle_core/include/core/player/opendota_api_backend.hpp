#ifndef OPENDOTA_API_BACKEND_H
#define OPENDOTA_API_BACKEND_H

#include <core/srvlog/srvlog.hpp>
#include <core/player/backend.hpp>

namespace oracle {
namespace Impl {

/*! \class opendota_api_backend
 *  \brief Brief class description
 *
 *  Detailed description
 */
class opendota_api_backend : public backend
{
public:
	opendota_api_backend(const player_id id);
	virtual ~opendota_api_backend() {}

	virtual std::string name() override;
	virtual std::future<fetch_t> fetchData() override;
};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: OPENDOTA_API_BACKEND_H */
