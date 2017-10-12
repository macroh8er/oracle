#include <iostream>

#include <core/srvlog/srvlog.hpp>
#include <core/fswatch/fswatch.hpp>
#include <overlay/overlay.hpp>

#include <thread>
#include <chrono>

int main(int argc, const char *argv[])
{
	oracle::srvlog log(oracle::log_path("/home/thomas/.steam/steam/steamapps/common/dota 2 beta/game/dota/server_log.txt"));
	oracle::fswatch watch("/home/thomas/.steam/steam/steamapps/common/dota 2 beta/game/dota/server_log.txt");

	watch.close_write.connect( [&]() {
			/*
			 * see fswatch/fswatch.hpp, close_write is not necessarily sent when the file is readable
			 * wait for the writing to be sent to disk
			 */
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

			bool match = log.parse();
			if (match) {
				std::cout << "new match" << std::endl;
			}
			else {
				std::cout << "not a match" << std::endl;
			}
	});

	/*
	 * fswatch is done in a seperate thread, keep main thread active
	 */

	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
