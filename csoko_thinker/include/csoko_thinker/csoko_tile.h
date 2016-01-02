#ifndef CSOKO_TILE
#define CSOKO_TILE



#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

namespace csoko_thinker{
	class CSokoTile
	{
		public:
			int x,y;
			bool hasObject, isGoal, isSolid;
		
			CSokoTile(int x, int y, bool isGoal, bool isSolid);
			void setObject();

			bool increaseLock(size_t r_nr);
			bool decreaseLock(size_t r_nr);
			bool isLockedTo(size_t r_nr);
			bool isLocked();
		private:
			size_t lock_level, robot_nr;

	};
}
#endif
