#include <core/player/opendota_api_backend.hpp>

namespace oracle {
namespace Impl {

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


std::future< std::string > opendota_api_backend::fetchName() {
	
}

std::future< int > opendota_api_backend::fetchSoloMMR() {

}	

std::future< int > opendota_api_backend::fetchPartyMMR() {

}

} /* namespace Impl */
} /* namespace oracle */


