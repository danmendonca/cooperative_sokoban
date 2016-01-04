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
	this->moveComplete = mvC;
	this->mState = STATE_MOV_NONE;
	toMoveX =x;
	toMoveY=y;
	mvX = false;
	mvY = false;
	dMov = 0;
}

void CSokoObject::updateDrawCoord()
{

	float err = 0.01;

	float dx = toMoveX - drawX;
	float dy = toMoveY - drawY;

	dx *= (dx < 0) ? -1 : 1;
	dy *= (dy < 0) ? -1 : 1;

	if(dx > err || dy > err)
	{
		if(toMoveX != drawX) drawX += 0.1 * dMov;
		else drawY += 0.1 * dMov;

		ROS_DEBUG("err= %f  dx= %f  dy= %f", err, dx, dy);
		ROS_DEBUG("0.1 * dMov = %f", 0.1*dMov);
		ROS_DEBUG("toMoveX: %f    toMoveY: %f", toMoveX, toMoveY);
		ROS_DEBUG("drawX  : %f    drawY  : %f", drawX, drawY);
		ROS_DEBUG("0.1 * dMov = %f", 0.1*dMov);
		//sleep(1);
	}
	else
	{
		ROS_DEBUG("movement complete");
		moveComplete= true;
		if(toMoveX != drawX) drawX += 0.1 * dMov;
				else drawY += 0.1 * dMov;
		x = toMoveX;
		y = toMoveY;
		toMoveX = drawX;
		toMoveY = drawY;
		dMov = 0;
	}

}

bool CSokoObject::lastMoveFinished()
{

	bool ans = false;
	if(moveComplete == true && mState == STATE_MOV_NONE)
	{
		ans = true;
	}
	else if(moveComplete == true && mState == STATE_MOV_PROGRESS)
	{
		ans = true;
		mState = STATE_MOV_FINISHED;
	}

	return ans;
}

void CSokoObject::addMove(float mx, float my)
{
	mState = STATE_MOV_PROGRESS;
	mvX = (mx > 0) ? true : false;
	mvY = (my > 0) ? true : false;
	toMoveX = mx + x;
	toMoveY = my + y;
	dMov = (mx > my) ? mx : my;
	this->moveComplete = false;
}
}
