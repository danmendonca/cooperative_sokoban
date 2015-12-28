#include "csoko_thinker/csoko_tile.h"

#include <QPointF>

namespace csoko_thinker{

	/***************************************
	*
	*Tile class
	*
	****************************************/
	CSokoTile::CSokoTile(int x, int y, bool isGoal, bool isSolid)
	{
		this->x = x;
		this->y = y;
		this->isGoal = isGoal;
		this->isSolid = isSolid;
		this->hasObject = false;
	}
	
	void CSokoTile::setObject(CSokoObject object)
	{
		this->object = object;
		this->hasObject = true;
	}
}
