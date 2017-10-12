#include <core/fswatch/fswatch.hpp>
#include <iostream>
#include <exception>
#include <thread>

namespace oracle {

struct fswatch::impl
{
	impl(fswatch* self, const std::string path);
	~impl();
};

#ifdef oracle_linux

/*
 * Use inotify
 */

#include <sys/inotify.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define EVSIZE (sizeof(struct inotify_event))
#define BUFLEN (1024 * (EVSIZE + 16))

fswatch::impl::impl(fswatch* self, const std::string path) {

	/*
	 * self is only used by us, it's thread safe
	 */

	std::thread spawned([self, path]() {

	int fd;
	int wd;
	char buffer[BUFLEN];
	
	fd = inotify_init();

	if (fd < 0) {
		throw std::runtime_error("Failed to initialise inotify");
	}

	wd = inotify_add_watch(fd, path.c_str(),
		IN_MODIFY | IN_CLOSE_WRITE);

	while(1) {

	int length = read(fd, buffer, BUFLEN);

	if (length < 0) {
		throw std::runtime_error("Failed to read inotify buffer");
	}

	int i=0;
	while (i < length) {
		inotify_event* event =
			(inotify_event*) &buffer[i];

		if (event->mask & IN_MODIFY) {
			self->modified();
		}
		else if (event->mask & IN_CLOSE_WRITE) {
			self->close_write();
		}

		i += EVSIZE + event->len;

	}

	}

	inotify_rm_watch(fd, wd);
	close(fd);

	});

	spawned.detach();
}

#endif

fswatch::impl::~impl() {

}

/*
 * fswatch interface
 */

fswatch::fswatch(const std::string file_path)
	: m_impl(std::make_unique<impl>(this, file_path))
{
}

fswatch::~fswatch() {
	
}

} /* namespace oracle */

