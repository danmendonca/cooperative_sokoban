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


namespace csoko_thinker{
	class CSokoObject
	{
	public:
		int x,y;
		double drawX, drawY;
		bool isBox;

		CSokoObject(int x, int y, bool isBox);
	};
}
#endif
