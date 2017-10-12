#ifndef OVERLAY_H
#define OVERLAY_H

#include <memory>

namespace oracle {

/*! \class overlay
 *  \brief Game overlay
 *
 *  Window with dota 2 game window as transient parent
 */
class overlay
{
public:
	/*
	 * q_argc must be > 0, see
	 * 	http://doc.qt.io/qt-5/qguiapplication.html#QGuiApplication
	 *
	 * it also must be a reference, see
	 * 	https://bugreports.qt.io/browse/QTBUG-5637
	 */
	overlay(const std::string content_dir, int& q_argc, char** q_argv);
	~overlay();

	/*
	 * blocks thread until completion
	 * 	TODO: maybe spawn a new thread like core/fswatch
	 * 	with callbacks
	 */
	int exec();

protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */
};

} /* namespace oracle */

#endif /* end of include guard: OVERLAY_H */
