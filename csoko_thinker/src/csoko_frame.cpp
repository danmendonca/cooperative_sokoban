#include "csoko_thinker/csoko_frame.h"


/**
 * Load map, create frame, start timer
 */
namespace csoko_thinker{

std::string CSokoFrame::path_to_res = "";

void CSokoFrame::setPathToResources(std::string p)
{
	CSokoFrame::path_to_res = p;
}

CSokoFrame::CSokoFrame()
{
	ROS_ERROR("WINDOW CREATE");
	window.create(sf::VideoMode( 500, 500 ), "CSOKO");
	ROS_ERROR("BEFORE FRAME LOAD");

	//LOAD GOAL
	string goalPath = path_to_res +"/images/goal.png";
	if (!goal.loadFromFile(goalPath))
	{
	    // error...
	}
	goalSprite.setTexture(goal);
	//LOAD BOX
	string boxPath = path_to_res +"/images/box.png";
	if (!boxIcon.loadFromFile(boxPath))
	{
	    // error...
	}
	//LOAD ROBOT
	string robotPath = path_to_res +"/images/robot.png";
	if (!robotIcon.loadFromFile(robotPath))
	{
	    // error...
	}

	ROS_ERROR("FRAME IS LOADED");
}

void CSokoFrame::closeWindow()
{
	//window.close();
}

void CSokoFrame::loadMap(string mapName)
{
	string bgPath = mapName;
	if (!bg.loadFromFile(bgPath))
	{
	    // error...
	}
	bgSprite.setTexture(bg);
	bgSprite.setPosition(sf::Vector2f(0, 0));
	window.setSize(bg.getSize());
}

void CSokoFrame::signalUpdate(vector<vector<CSokoTile> > grid, vector<CSokoObject> objects)
{
	this->grid = grid;
	this->objects = objects;
	draw();
}

void CSokoFrame::draw()
{
	if(window.isOpen())
	{
        window.clear();
		//Draw Background
		if(bg.getSize().x != 0)
		{
			window.draw(bgSprite);
		}

		//Draw Goal
		for(int i=0;i<grid.size();i++)
		{
			for(int j=0;j<grid[i].size();j++)
			{
				CSokoTile tile = grid[i][j];
				if(tile.isGoal)
				{
					if(goal.getSize().x != 0)
					{
						goalSprite.setPosition(sf::Vector2f(j*16, i*16));
						window.draw(goalSprite);
					}
				}
			}
		}
		sf::Sprite dynSprite;
		//Draw Robots and Boxes
		for(int j=0;j<objects.size();j++)
		{
			if(boxIcon.getSize().x != 0 & robotIcon.getSize().x != 0)
			{
				if(objects[j].isBox)
					dynSprite.setTexture(boxIcon);
				else
					dynSprite.setTexture(robotIcon);
				dynSprite.setPosition(sf::Vector2f(objects[j].drawX*16, objects[j].drawY*16));
				window.draw(dynSprite);
			}
		}
        window.display();
	}
}

}
