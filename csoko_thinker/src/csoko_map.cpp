#include "csoko_thinker/csoko_map.h"

#include <QPointF>

namespace csoko_thinker{

	/***************************************
	*
	*SokobanObject class
	*
	****************************************/
	CSokoObject::CSokoObject(int x, int y)
	{
		this->x = x;
		this->y = y;
		drawX = x;
		drawY = y;
	}
	
	CSokoRobot::CSokoRobot(int x, int y) : CSokoObject(x,y)
	{
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "robot";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
	}
	
	CSokoBox::CSokoBox(int x, int y) : CSokoObject(x,y)
	{
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "box";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
	}
	
	/***************************************
	*
	*Tile class
	*
	****************************************/
	CSokoTile::CSokoTile(int x, int y, bool isGoal, bool isSolid)
	{
		this->x = x;
		this->y = y;
		this->isGoal = isGoal;
		this->isSolid = isSolid;
		this->hasObject = false;
	}
	
	void CSokoTile::setObject(CSokoObject object)
	{
		this->object = object;
		this->hasObject = true;
	}
		
	/***************************************
	*
	*Map class
	*
	****************************************/
	CSokoMap::CSokoMap(string mapName)
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
						tile.setObject(CSokoRobot());
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
	
	void CSokoMap::drawAll(CSokoFrame frame)
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
