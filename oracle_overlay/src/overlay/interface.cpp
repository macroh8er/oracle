#include "interface.h"

#include <cstdlib>

#include <spdlog/fmt/fmt.h>

/*
 * for Qt version
 */
#include <QtGlobal>

#include <QLabel>
#include <QPixmap>

#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

namespace oracle {
namespace Impl {

enum player_colors {
	PC_BLUE,
	PC_WHITE,
	PC_PURPLE,
	PC_YELLOW,
	PC_ORANGE,

	PC_PINK,
	PC_GREY,
	PC_LBLUE,
	PC_GREEN,
	PC_BROWN
};

/*
 * TODO: the gradients aren't exact, find a relationship between the first color gradient
 * 	(PC_BLUE)'s first rgba and second rgba
 */

constexpr const char* player_color_gradients[] = {
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(25, 59, 131, 255), stop:0.476923 rgba(59, 112, 223, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(58, 149, 110, 255), stop:0.476923 rgba(84, 213, 159, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(102, 0, 102, 255), stop:0.476923 rgba(158, 0, 158, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(135, 133, 3, 255), stop:0.476923 rgba(202, 200, 7, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(132, 53, 0, 255), stop:0.476923 rgba(213, 88, 0, 255))",

	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(115, 60, 88, 255), stop:0.476923 rgba(212, 111, 162, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(90, 100, 38, 255), stop:0.476923 rgba(134, 150, 58, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(53, 115, 131, 255), stop:0.476923 rgba(83, 181, 206, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(0, 71, 15, 255), stop:0.476923 rgba(0, 108, 26, 255))",
	"qlineargradient(spread:reflect, x1:0, y1:0, x2:0.492385, y2:0, stop:0 rgba(86, 53, 0, 255), stop:0.476923 rgba(136, 86, 0, 255))"
};


/*
 * player color strip
 */

class player_color : public QWidget {
public:
	/*
	 * where color = PC_*, player_colors
	 */
	explicit player_color(int color, int w, int h, QWidget* parent = 0);
	virtual ~player_color() = default;
};

player_color::player_color(int color, int w, int h, QWidget* parent)
	: QWidget(parent)
{
	setFixedSize(w,h);

	setAutoFillBackground(true);

	/*
	 * build stylesheet
	 */

	std::string stylesheet = fmt::format("background-color: {0}",
			player_color_gradients[color]
			);

	setStyleSheet( QString::fromStdString(stylesheet) );
}

/*
 * holds the content
 */
class inner_interface : public QWidget {
public:
	explicit inner_interface(int w, int h, QWidget* parent = 0);
	virtual ~inner_interface() = default;

private:
	std::array<player_color*, 10> m_player_colors;

};

inner_interface::inner_interface(int w, int h, QWidget* parent)
	: QWidget(parent)
{
	setFixedSize(w, h);
	
	/*
	 * center element
	 */
	int x = (parent->size().width() / 2) - (w / 2);
	int y = (parent->size().height() / 2) - (h / 2);
	setGeometry(x, y, w, h);
	setAutoFillBackground(true);

	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
	shadow->setBlurRadius(30.0);
	shadow->setColor(QColor(0,0,0,255));
	shadow->setOffset(0.0f);

	setGraphicsEffect(shadow);

	/*
	 * stylesheet for inner_interface
	 */
	setObjectName("inner_interface");

	setStyleSheet(QStringLiteral("#inner_interface { background-color: #212125; border-style: solid; border-width: 5px; border-color: #2a2d2d; }"));


	for (int i = 0; i < 10; i++) {
		m_player_colors[i] = new player_color(i, 600, 20, this);
		m_player_colors[i]->setGeometry(100, i * 40, 600, 20);
		m_player_colors[i]->show();
	}
	

}

/*
 * interface root
 */

interface::interface(QWidget *parent)
	: QFrame(parent)
{
	//connect(engine(), SIGNAL(quit()), SLOT(close()));
	setFixedSize(1000, 600);

	set_frameless();
	set_background();

	inner_interface* inner = new inner_interface(950, 550, this);
	inner->show();
	
}

void interface::set_background() {
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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

