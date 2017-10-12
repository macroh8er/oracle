#include <iostream>

#include <core/srvlog/srvlog.hpp>
#include <core/fswatch/fswatch.hpp>
#include <overlay/overlay.hpp>

#include <thread>
#include <chrono>
#include <sstream>

int main(int argc, char *argv[])
{
	oracle::srvlog log(oracle::log_path("/home/thomas/.steam/steam/steamapps/common/dota 2 beta/game/dota/server_log.txt"));
	oracle::fswatch watch("/home/thomas/.steam/steam/steamapps/common/dota 2 beta/game/dota/server_log.txt");

	/*
	 * get dir of the executable
	 *
	 * non unix paths might be \\ instead of /
	 */
	std::string ex_path = argv[0];
	ex_path = ex_path.substr(
			0,
			ex_path.find_last_of('/') + 1
			);

	std::stringstream abs_path;
	abs_path << ex_path;

	oracle::overlay overlay(abs_path.str(), argc, argv);
	return overlay.exec();

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
