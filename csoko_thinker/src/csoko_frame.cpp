#include "csoko_thinker/csoko_frame.h"

sf::Texture bg;
			sf::Texture robotIcon;
			sf::Texture boxIcon;
			sf::Texture goal;

using namespace std;

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
	ROS_DEBUG("WINDOW CREATE");
	window.create(sf::VideoMode( 500, 500 ), "CSOKO");
	ROS_DEBUG("BEFORE FRAME LOAD");

	//LOAD GOAL
	string goalPath = path_to_res +"/images/goal.png";
	if (!goal.loadFromFile(goalPath))
	{
		ROS_DEBUG("COULD NOT LOAD MAP GRAPHICS");
	}
	//LOAD BOX
	string boxPath = path_to_res +"/images/box.png";
	if (!boxIcon.loadFromFile(boxPath))
	{
		ROS_ERROR("COULD NOT LOAD BOX GRAPHICS");
	}
	//LOAD ROBOT
	string robotPath = path_to_res +"/images/robot.png";
	if (!robotIcon.loadFromFile(robotPath))
	{
		ROS_ERROR("COULD NOT LOAD ROBOT GRAPHICS");
	}

	ROS_INFO("FRAME IS LOADED");
}

void CSokoFrame::closeWindow()
{
	//window.close();
}

void CSokoFrame::loadMap(string mapName)
{
	mName = mapName;

	bgSprite.setPosition(sf::Vector2f(0, 0));
	//window.setSize(bg.getSize());
}

void CSokoFrame::signalUpdate(vector<vector<CSokoTile> > grid, vector<CSokoObject> objects)
{
	this->grid = grid;
	this->objects = objects;
	draw();
}

void CSokoFrame::draw()
{
		window.clear(sf::Color::White);
		//Draw Background
		string bgPath = mName;
		if (!bg.loadFromFile(bgPath+".png"))
		{
		    // error...
		}
		bgSprite.setTexture(bg);
		window.draw(bgSprite);
		if(bg.getSize().x != 0)
		{

		}
		else
		{
			cout << "Map graphics not loaded" << endl;
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
						//cout << "Drawing goal" << endl;
						goalSprite.setTexture(goal);
						goalSprite.setPosition(sf::Vector2f(j*16, i*16));
						window.draw(goalSprite);
					}
					else
						cout << "Goal sprite not loaded" << endl;
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
