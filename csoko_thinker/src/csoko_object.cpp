#include "csoko_thinker/csoko_object.h"

namespace csoko_thinker{
/***************************************
 *
 *SokobanObject class
 *
 ****************************************/
CSokoObject::CSokoObject(int x, int y, bool isBox, bool mvC)
{
	this->x = x;
	this->y = y;
	drawX = x;
	drawY = y;
	this->isBox = isBox;
	this->mState = STATE_MOV_NONE;
	toMoveX =x;
	toMoveY=y;
	mvX = false;
	mvY = false;
}

void CSokoObject::updateDrawCoord()
{

	bool enoughY = false;
	bool enoughX = false;

	ROS_DEBUG("X whereTo: %f whereNow: %f", toMoveX, drawX);
	ROS_DEBUG("Y whereTo: %f whereNow: %f", toMoveY, drawY);

	if(mvX)
	{
		drawX += 0.1;
		enoughY = true;

		if(toMoveX > drawX)
		{
			enoughX = ((toMoveX - drawX) < 0.1) ? true : false;
		}
		else
		{
			enoughX = ((drawX - toMoveX) < 0.1) ? true : false;
		}

	}
	if(mvY)
	{
		enoughX = true;
		drawY += 0.1;

		if(toMoveY > drawY)
		{
			enoughY = ((toMoveX - drawY) < 0.1) ? true : false;
		}
		else
		{
			enoughY = ((drawY - toMoveY) < 0.1) ? true : false;
		}
	}



	if(enoughX && enoughY)
	{
		this->mState = STATE_MOV_FINISHED;

		if(mvX)
			x += 1;
		if(mvY)
			y+=1;
		mvX=false;
		mvY=false;
		ROS_DEBUG("movement complete");
		sleep(3);
	}
}

void CSokoObject::addMove(float mx, float my)
{
	mState = STATE_MOV_PROGRESS;
	mvX = (mx > 0) ? true : false;
	mvY = (my > 0) ? true : false;
	toMoveX = x + mx;
	toMoveY = y + my;
}
}
