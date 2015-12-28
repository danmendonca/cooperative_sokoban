#ifndef CSOKO_MAP
#define CSOKO_MAP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

namespace csoko_thinker
{

class SokobanObject
{
	public:
		SokobanObject();
};

class Tile
{
	public:
		int x,y;
		SokobanObject object;
		bool hasObject, isGoal, isSolid;
		
		Tile(int x, int y, bool isGoal, bool isSolid);
		
};


class Map
{
	public:
		std::string mapName;
		std::vector<std::vector<Tile> > grid;	
		Map(std::string mapName);	
};



}
#endif