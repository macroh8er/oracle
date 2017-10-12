#ifndef INTERFACE_H
#define INTERFACE_H

#include <memory>
#include <QtQuick/QQuickView>

namespace oracle {
namespace Impl {

/*! \class interface
 *  \brief Brief class description
 *
 *  Detailed description
 */
class interface : public QQuickView
{
	Q_OBJECT
public:
	explicit interface(QWindow *parent = 0);
	virtual ~interface();

	void setMainQmlPath(const QString &path);

	void showExpanded();

protected:
	struct impl;
	std::unique_ptr<impl> m_impl; /* pImpl */
};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: INTERFACE_H */

