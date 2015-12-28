#include "csoko_thinker/csoko_thinker.h"

#include <QPointF>

namespace csoko_thinker{
	/***************************************
	*
	*Map class
	*
	****************************************/
	CSoko_Map::CSoko_Map(string mapName)
	{
		string line;
		int row = 0;
		string mapFilePath = mapName + "_info.txt";
		
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + mapName;
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		bg.load(images_path);

		s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "goal";
		images_path = QString::fromAscii(s.c_str(), s.length());
		this->goal.load(images_path);

		
		ifstream mapFile(mapFilePath.c_str());
		if(mapFile.is_open())
		{
			while(getline(mapFile,line))
			{
				std::vector<CSokoTile> mapRow;
				for(int i=0;i<line.length(); i++)
				{
					if(line[i] == '-')
					{
						mapRow.push_back(CSokoTile(i,row,false, false));
					}
					else if(line[i] == 'O')
					{
						mapRow.push_back(CSokoTile(i,row,true, false));
					}
					else if(line[i] == 'R')
					{
						CSokoTile tile = CSokoTile(i,row,false, false);
						tile.setObject(CSoko_Robot());
						mapRow.push_back(tile);
					}
					else if(line[i] == 'C')
					{
						CSokoTile tile = CSokoTile(i,row,false, false);
						tile.setObject(CSokoBox());
						mapRow.push_back(tile);
					}
					else if(line[i] == 'P')
					{
						mapRow.push_back(CSokoTile(i,row,false, true));
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
	
	void CSoko_Map::drawAll(CSokoFrame frame)
	{
		frame.draw(bg,QPointF(0,-bg.height()));
		for(int i=0;i<grid.size();i++)
		{
			for(int j=0;j<grid[i].size();j++)
			{
				CSokoTile tile = grid[i][j];
				if(tile.isGoal)
				{
					frame.draw(tile.object.icon,QPointF(j,goal.height()-i*16));		//TODO MAGIC NUMBER
				}
				
				if(tile.hasObject)
					frame.draw(tile.object.icon,QPointF(tile.object.drawX,bg.height()-tile.object.drawY));
			}
		}
	}
}
