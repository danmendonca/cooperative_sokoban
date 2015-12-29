#include "csoko_thinker/csoko_object.h"

namespace csoko_thinker{
	/***************************************
	*
	*SokobanObject class
	*
	****************************************/
	CSokoObject::CSokoObject(int x, int y, bool isBox)
	{
		this->x = x;
		this->y = y;
		drawX = x;
		drawY = y;
		this->isBox = isBox;
	}
}
