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
	delta = 0;
}

void CSokoObject::updateDrawCoord()
{

	bool enoughY = false;
	bool enoughX = false;

	ROS_DEBUG("X whereTo: %f whereNow: %f", toMoveX, drawX);
	ROS_DEBUG("Y whereTo: %f whereNow: %f", toMoveY, drawY);

	if(mvX)
	{
		drawX += (drawX > toMoveX) ? -0.01 : 0.01;
		enoughY = true;

		if(toMoveX > drawX)
		{
			enoughX = ((toMoveX - drawX) < 0.01) ? true : false;
			delta = 1;
		}
		else
		{
			enoughX = ((drawX - toMoveX) < 0.01) ? true : false;
			delta = -1;
		}

	}
	if(mvY)
	{
		enoughX = true;

		drawY += (drawY > toMoveY) ? -0.01 : 0.01;

		if(toMoveY > drawY)
		{
			delta = 1;
			enoughY = ((toMoveY - drawY) < 0.01) ? true : false;
		}
		else
		{
			delta = -1;
			enoughY = ((drawY - toMoveY) < 0.01) ? true : false;
		}
	}



	if(enoughX && enoughY)
	{
		this->mState = STATE_MOV_FINISHED;

		if(mvX)
			x += delta;
		if(mvY)
			y+=delta;
		mvX=false;
		mvY=false;
		delta = 0;

		ROS_DEBUG("movement complete");
		//sleep(3);
	}
}

void CSokoObject::addMove(float mx, float my)
{
	mState = STATE_MOV_PROGRESS;
	mvX = (mx != 0) ? true : false;
	mvY = (my != 0) ? true : false;
	toMoveX = drawX + mx;
	toMoveY = drawY + my;
}
}
