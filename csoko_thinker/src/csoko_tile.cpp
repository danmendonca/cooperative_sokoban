#include "csoko_thinker/csoko_tile.h"

using namespace std;
namespace csoko_thinker{
	CSokoTile::CSokoTile(int x, int y, bool isGoal, bool isSolid)
	{
		this->x = x;
		this->y = y;
		this->isGoal = isGoal;
		this->isSolid = isSolid;
		this->hasObject = false;
		this->lock_level = 0;
		this->robot_nr = numeric_limits<size_t>::max();
	}
	
	void CSokoTile::setObject()
	{
		this->hasObject = true;
	}

	bool CSokoTile::increaseLock(size_t r_nr)
	{
		if(lock_level != 0 && robot_nr != r_nr)
			return false;

		lock_level++;
		robot_nr = r_nr;
		return true;
	}

	bool CSokoTile::decreaseLock(size_t r_nr)
	{
		if(robot_nr != r_nr)
			return false;

		lock_level = (lock_level < 0) ? 0 : lock_level -1;
		if(lock_level == 0)
			robot_nr = numeric_limits<size_t>::max();

		return true;
	}

	bool CSokoTile::isLocked()
	{
		return (lock_level != 0) ? true : false;
	}

	bool CSokoTile::isLockedTo(size_t r_nr)
	{
		return (robot_nr == r_nr) ? true : false;
	}
}
