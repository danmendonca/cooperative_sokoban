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
	this->mState = STATE_MOV_NONE;
	toMoveX =x;
	toMoveY=y;
	mvX = false;
	mvY = false;
	delta = 0;
	facing = 0;		//2 - left, 3 - up, 0 - right, 1 - down
	rotation = 0.f;
}


bool CSokoObject::updateDrawCoord()
{

	bool enoughY = false;
	bool enoughX = false;

	//rotation = 9;

	if(!isBox){


		if(rotation > 360)
			rotation = 0;


		if(facing == 3 && rotation != 270 )
		{
			if(rotation > 270)
				rotation -= 1;
			else
				rotation +=1;
			return false;
		}
		else if(facing == 1 && rotation != 90 )
		{
			if(rotation > 90)
				rotation -= 1;
			else
				rotation +=1;
			return false;
		}
		else if(facing == 2 && rotation != 180 )
		{
			if(rotation > 180)
				rotation -= 1;
			else
				rotation +=1;
			return false;
		}
		else if(facing == 0 && rotation != 0)
		{
			if(rotation > 0)
				rotation -= 1;
			else
				rotation +=1;
			return false;
		}
	}

	//rotation += 0.5;
	//return;

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
	}

	return true;
}

void CSokoObject::addMove(float mx, float my)
{
	mState = STATE_MOV_PROGRESS;
	mvX = (mx != 0) ? true : false;
	mvY = (my != 0) ? true : false;
	toMoveX = drawX + mx;
	toMoveY = drawY + my;

	if(mx > 0 )
		facing  = 0;
	else if(mx < 0 )
		facing  = 2;
	else if(my > 0 )
		facing  = 1;
	else if(my < 0 )
		facing  = 3;
}
}
