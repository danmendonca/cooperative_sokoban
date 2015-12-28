#include "csoko_thinker/csoko_frame.h"

#include <QPointF>

/**
 * Load map, create frame, start timer
 */
namespace csoko_thinker{

CSokoFrame::CSokoFrame(QWidget* parent, Qt::WindowFlags f) : QFrame(parent, f)
, frame_count_(0)
{
	
}

void CSokoFrame::draw(QImage img, QPointF pos)
	QPainter painter(this);
	QPointF p = pos* img.height();
	painter.drawImage(p,img);
}
