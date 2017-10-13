#include <core/player/backend.hpp>
#include <core/logging/logging.hpp>

#include <exception>

/*
 * use cURL
 */
#include <curl/curl.h>

namespace oracle {
namespace Impl {

backend::backend(const player_id id)
	: m_id(id)
{
	
}

static std::size_t curl_write_cb(char* ptr, std::size_t size, std::size_t nmemb, void* userdata) {
	std::string& str = *( static_cast<std::string*>(userdata) );
	
	for (int i = 0; i < size; i++) {
		str += ptr[i];
	}

	return (size * nmemb);
}

std::string backend::GET(const std::string url) {

	/*
	 * At the moment using cURL
	 * TODO: switch to cURL multi interface, would require
	 * more OS specific code
	 */

	std::string retval = "";
	CURL* curl = curl_easy_init();

	if (!curl) {
		oracle::logging::get().error("Failed to init cURL");
		throw std::runtime_error("Failed to init cURL");
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &retval);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		oracle::logging::get().error("cURL failed to perform operation: {0}",
				curl_easy_strerror(res));
		throw std::runtime_error("cURL failed to perform operation");
	}

	curl_easy_cleanup(curl);
	return retval;

}

json backend::GETjson(const std::string url) {

	std::string resp = GET(url);
	return json::parse(resp);

}

} /* namespace Impl */
} /* namespace oracle */
