#ifndef CSOKO_TILE
#define CSOKO_TILE



#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

namespace csoko_thinker{
	class CSokoTile
	{
		public:
			int x,y;
			bool hasObject, isGoal, isSolid;
		
			CSokoTile(int x, int y, bool isGoal, bool isSolid);
			void setObject();
	};
}
#endif
