#include "csoko_thinker/csoko_frame.h"

#include <QPointF>

/**
 * Load map, create frame, start timer
 */
namespace csoko_thinker{

CSokoFrame::CSokoFrame() : QFrame(0, 0)
, frame_count_(0)
{
	
}

CSokoFrame::CSokoFrame(QWidget * parent) : QFrame(0, 0)
, frame_count_(0)
{
	
}

void CSokoFrame::clear()
{
}

void CSokoFrame::paintEvent(QPaintEvent*e)
{
}

void CSokoFrame::draw(QImage img, QPointF pos)
{
	QPainter painter(this);
	QPointF p = pos* img.height();
	painter.drawImage(p,img);
}

}
