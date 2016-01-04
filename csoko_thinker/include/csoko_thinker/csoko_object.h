#ifndef CSOKO_OBJECT
#define CSOKO_OBJECT


#include "ros/ros.h"
#include <ros/package.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <ctime>


#define STATE_MOV_NONE 0
#define STATE_MOV_PROGRESS 1
#define STATE_MOV_FINISHED 2

namespace csoko_thinker{
	class CSokoObject
	{
	public:
		int x,y;
		float drawX, drawY;
		bool isBox;
		bool moveComplete;
		bool mvX, mvY;
		float toMoveX;
		float toMoveY;
		float dMov;
		int mState;

		void updateDrawCoord();
		void addMove(float mx, float my);
		bool lastMoveFinished();

		CSokoObject(int x, int y, bool isBox, bool mvC= false);
	};
}
#endif
