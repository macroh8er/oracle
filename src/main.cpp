#include <iostream>

#include <core/logging/logging.hpp>
#include <core/srvlog/srvlog.hpp>
#include <core/fswatch/fswatch.hpp>
#include <overlay/overlay.hpp>
#include <core/env/env.hpp>

#include <thread>
#include <chrono>
#include <sstream>

int main(int argc, char *argv[])
{

	/*
	 * load oracle environment
	 */
	oracle::env::parse(argc, argv);
	if(oracle::env::get().needsHelp()) {
		oracle::env::get().printOptions();
		return 0;
	}

	/*
	 * if using --verbose set logging level to debug
	 */
	if (oracle::env::get().isVerbose()) {
		oracle::logging::get().set_level(spdlog::level::debug);
	}

	/*
	 * if using --silent set logging level to critical
	 */
	if (oracle::env::get().isSilent()) {
		oracle::logging::get().set_level(spdlog::level::critical);
	}

	oracle::logging::get().info("Oracle starting");
	oracle::logging::get().info("For up to date bug-fixes and features clone the oracle repository: https://github.com/macroh8er/oracle");

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
				oracle::logging::get().info("Game initated");
				std::vector<oracle::player_id> players = log.getPlayers();
				for (oracle::player_id i : players) {
					oracle::logging::get().debug("ID: {0}\t\tteam: {1}\tparty: {2}", i.id, i.team, i.party);
				}
			}
			/*
			 * ignore
			 */
	});

	/*
	 * fswatch is done in a seperate thread, keep main thread active
	 */

	while (1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
