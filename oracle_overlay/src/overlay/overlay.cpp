#include <QtGui/QGuiApplication>
#include <overlay/overlay.hpp>

#include "interface.h"

namespace oracle {

struct overlay::impl
{

	std::unique_ptr<QGuiApplication> application;

	impl(int q_argc, char** q_argv);
	~impl() {}
};


overlay::impl::impl(int q_argc, char** q_argv) {

	application = std::make_unique<QGuiApplication>(
			q_argc, q_argv
			);

	Impl::interface viewer;

	viewer.setMainQmlPath(QStringLiteral("config/qml/main.qml"));
	viewer.setFlags(Qt::FramelessWindowHint);
	viewer.showExpanded();
}

/*
 * interface
 */

overlay::overlay(int q_argc, char** q_argv)
	: m_impl(std::make_unique<impl>(q_argc, q_argv))
{

}

int overlay::exec() {
	return m_impl->application->exec();
}

overlay::~overlay() = default;


} /* namespace oracle */

