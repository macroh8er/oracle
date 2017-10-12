#ifndef INTERFACE_H
#define INTERFACE_H

#include <memory>
#include <QWidget>

namespace oracle {
namespace Impl {

/*! \class interface
 *  \brief Brief class description
 *
 *  Detailed description
 */
class interface : public QWidget
{
public:
	explicit interface(QWidget *parent = 0);
	virtual ~interface();

private:

	/*
	 * set necessary options and attributes
	 * to be a frameless window
	 */
	void set_frameless();

	void set_background();
};

} /* namespace Impl */
} /* namespace oracle */

#endif /* end of include guard: INTERFACE_H */

