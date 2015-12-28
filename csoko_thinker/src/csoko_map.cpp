#include "csoko_thinker/csoko_map.h"


namespace csoko_thinker{

	/***************************************
	*
	*SokobanObject class
	*
	****************************************/
	SokobanObject::SokobanObject(){}
	
	/***************************************
	*
	*Tile class
	*
	****************************************/
	Tile::Tile(int x, int y, bool isGoal, bool isSolid)
		{
			this->x = x;
			this->y = y;
			this->isGoal = isGoal;
			this->isSolid = isSolid;
		}
		
	/***************************************
	*
	*Map class
	*
	****************************************/
	Map::Map(std::string mapName)
		{
			std::string line;
			int row = 0;
			std::string mapFilePath = mapName + "_info.txt";
			std::ifstream mapFile(mapFilePath.c_str());
			if(mapFile.is_open())
			{
				while(getline(mapFile,line))
				{
					std::vector<Tile> mapRow;
					for(int i=0;i<line.length(); i++)
					{
						if(line[i] == '-')
						{
							mapRow.push_back(Tile(i,row,false, false));
						}
						else if(line[i] == 'O')
						{
							mapRow.push_back(Tile(i,row,true, false));
						}
						else if(line[i] == 'R')
						{
							//ADD ROBOT TO TILE
							mapRow.push_back(Tile(i,row,false, false));
						}
						else if(line[i] == 'C')
						{
							//ADD BOX TO TILE
							mapRow.push_back(Tile(i,row,false, false));
						}
						else if(line[i] == 'P')
						{
							mapRow.push_back(Tile(i,row,false, true));
						}
					}
					grid.push_back(mapRow);
					row++;
				}
				mapFile.close();
			}
			else {
				cout << "Unable to find map file." << endl;
			}
		}
	
	
}
