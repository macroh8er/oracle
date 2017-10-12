#include "interface.h"

namespace oracle {
namespace Impl {

struct interface::impl
{

	QString mainQmlPath;

	impl();
	~impl();
};

interface::interface(QWindow *parent)
	: QQuickView(parent),
	  m_impl(std::make_unique<impl>())
{
	connect(engine(), SIGNAL(quit()), SLOT(close()));
	setResizeMode(QQuickView::SizeRootObjectToView);
}

void interface::setMainQmlPath(const QString &path) {
	/*
	 * may need to adjust path on different platforms
	 * 	https://github.com/wang-bin/QtAV/blob/master/examples/QMLPlayer/qtquick2applicationviewer/qtquick2applicationviewer.cpp
	 */

	m_impl->mainQmlPath = path;
	setSource(m_impl->mainQmlPath);
}

void interface::showExpanded() {
	show();
}

interface::~interface() = default;

} /* namespace Impl */
} /* namespace oracle */

