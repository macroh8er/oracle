#include <QApplication>
#include <overlay/overlay.hpp>

#include <sstream>

#include "interface.h"

namespace oracle {

struct overlay::impl
{

	std::unique_ptr<QApplication> application;

	impl(const std::string content_dir, int& q_argc, char** q_argv);
	~impl() {}
};


overlay::impl::impl(const std::string content_dir, int& q_argc, char** q_argv) {

	application = std::make_unique<QApplication>(
			q_argc, q_argv
			);
}

/*
 * interface
 */

overlay::overlay(const std::string content_dir, int& q_argc, char** q_argv)
	: m_impl(std::make_unique<impl>(content_dir, q_argc, q_argv))
{

}

int overlay::exec() {
	Impl::interface viewer;

	viewer.show();

	return m_impl->application->exec();
}

overlay::~overlay() = default;


} /* namespace oracle */

