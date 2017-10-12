#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

#include <core/fswatch/fswatch.hpp>
#include <gtest/gtest.h>

TEST (fswatch, close_write) {
	/*
	 * create file to be watched for modifications
	 */
	std::ofstream file("t_fswatch.txt");
	file.close();

	oracle::fswatch watch("t_fswatch.txt");

	std::chrono::system_clock::time_point sent;

	watch.close_write.connect( [&]() {
			/*
			 * assert that the time of receiving is a similar time to the sent
			 */
			auto now = std::chrono::system_clock::now();
			auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(now - sent).count();

			ASSERT_EQ (delay, 0);
	});

	/*
	 * fswatch is done in a seperate thread, keep main thread active
	 */

	for (int i = 0; i < 10; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		sent = std::chrono::system_clock::now();

		file.open("t_fswatch.txt");
		file << "data ";
		file.close();
	}

}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
