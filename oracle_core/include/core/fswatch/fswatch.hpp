#ifndef FSWATCH_H
#define FSWATCH_H

#include <string>
#include <memory>

#include <boost/signals2.hpp>

namespace oracle {

/*! \class fswatch
 *  \brief Watch for filesystem events
 *
 *  Be alerted when filesystem events happen, using any, all or some of the following flags:
 *  	created		- filesystem path is created
 *  	deleted		- filesystem path is deleted
 *  	modified	- filesystem path is modified
 *  	close_write 	- filesystem path has been finished writing to
 *
 *  close_write doesn't necessarily fire when the file is ready to be read, it just
 *  means the kernel is closing a connection to the file. If you want to read after
 *  getting this event, wait a little bit.
 */
class fswatch
{
public:
	fswatch(const std::string file_path);
	~fswatch();

	boost::signals2::signal<void ()> modified;
	boost::signals2::signal<void ()> close_write;
	boost::signals2::signal<void ()> created;
	boost::signals2::signal<void ()> deleted;

protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */
};

} /* namespace oracle */

#endif
