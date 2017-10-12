#include "interface.h"

#include <cstdlib>

/*
 * for Qt version
 */
#include <QtGlobal>

#include <QLabel>
#include <QPixmap>

namespace oracle {
namespace Impl {

interface::interface(QWidget *parent)
	: QWidget(parent)
{
	//connect(engine(), SIGNAL(quit()), SLOT(close()));
	setFixedSize(600, 350);

	set_frameless();
	set_background();

	QLabel* img = new QLabel(this);
	img->setPixmap(QPixmap("build/media/logo_medium.png"));
	img->show();
	
}

void interface::set_background() {
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
}

void interface::set_frameless() {
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
	setParent(0); // Create TopLevel-Widget
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_TranslucentBackground, true);

	const char* qt_version = qVersion();
	float qt_version_n = std::atof(qt_version);

	if (qt_version_n <= 5.2) {
		setAttribute(Qt::WA_PaintOnScreen); // not needed in Qt 5.2 and up
	}
}

interface::~interface() = default;

} /* namespace Impl */
} /* namespace oracle */

