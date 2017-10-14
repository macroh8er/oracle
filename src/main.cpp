#include <iostream>

#include <core/logging/logging.hpp>
#include <core/srvlog/srvlog.hpp>
#include <core/fswatch/fswatch.hpp>
#include <overlay/overlay.hpp>
#include <core/env/env.hpp>
#include <core/player/player.hpp>

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

	oracle::overlay overlay("build/content", argc, argv);
	return overlay.exec();

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
				std::vector<oracle::player_id> player_ids = log.getPlayers();
				std::vector<std::unique_ptr<oracle::player>> players;

				/*
				 * load player data
				 */
				for (auto& i : player_ids) {
					oracle::logging::get().debug("ID {0}", i.id);
					players.push_back( std::make_unique<oracle::player>(i) );
				}

				/*
				 * now that player data is being fetched, start consuming it
				 * first, remove any players that don't have exposed profiles
				 */
				for (auto it = players.begin(); it != players.end(); ) {

					/*
					 * wait for data to arrive
					 */
					(*it)->waitForData();

					if ((*it)->getName() == "") {
						it = players.erase(it);
					}
					else {
						it++;
					}
				}

				for (auto& i : players) {
					oracle::logging::get().debug("Player {0} ({1})", i->getName(), i->id());

					if (i->isSoloEstimated()) {
						oracle::logging::get().debug("\tSolo MMR (Estimated*): {0}", i->getSoloMMR());
					}
					else {
						oracle::logging::get().debug("\tSolo MMR: {0}", i->getSoloMMR());
					}

					if (i->getPartyMMR()) {
						oracle::logging::get().debug("\tPaty MMR: {0}", i->getPartyMMR());
					}
				}

				oracle::logging::get().debug("*: This is not necessarily accurate, this user does not have a real MMR");


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
