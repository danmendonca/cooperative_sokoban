#ifndef CSOKO_FRAME
#define CSOKO_FRAME


#include <iostream>
#include <cstdlib>
#include <cmath>

#include <ros/package.h>
#include <ros/ros.h>

#include "csoko_thinker/csoko_object.h"
#include "csoko_thinker/csoko_tile.h"

#include <stdr_msgs/RobotIndexedVectorMsg.h>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>

#include <SFML/Graphics.hpp>

#include <map>

#include <iostream>
#include <memory>
#include <fstream>

//using namespace std;

namespace csoko_thinker
{
	class CSokoFrame
	{
		public:
			CSokoFrame();

			void signalUpdate(std::vector<std::vector<CSokoTile> > grid, std::vector<CSokoObject> objects);
			void draw();

			void loadMap(std::string mapName);

			void closeWindow();

			static std::string path_to_res;

			static void setPathToResources(std::string);


		private:
			sf::Sprite bgSprite;
			
			sf::RenderWindow window;
			std::string mName;
			std::vector<CSokoObject> objects;
			std::vector<std::vector<CSokoTile> > grid;
			sf::Sprite goalSprite;

	};
}
#endif
