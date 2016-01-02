#include "csoko_thinker/csoko_thinker.h"
using namespace csoko_thinker;
using namespace std;

int main(int argc,char **argv)
{
	if(csoko_thinker::CSoko_Thinker::CSOKO_THINKER_DEBUG)
	{

		if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug) ) {
			ros::console::notifyLoggerLevelsChanged();
		}
	}

	ros::init(argc, argv, "csoko_thinker_node", ros::init_options::AnonymousName);
	string res_path(argv[1]);
	CSokoFrame::setPathToResources(res_path);
	CSoko_Thinker obj(argc, argv);
	ros::spin();

	while(true)
	{}	
	return 0;
}

/**
 * Load map, create frame, start timer
 */
namespace csoko_thinker{

CSoko_Thinker::CSoko_Thinker(int argc,char **argv)
{
	odom_state = 0;
	string res_path(argv[1]);
	string name = res_path + argv[2];

	CSokoFrame::setPathToResources(res_path);

	ROS_INFO("MAP NAME: %s\n", name.c_str());

	CSokoFrame::setPathToResources(res_path);

	loadMap(name);

	//	ros::Timer timer = nh.createTimer(ros::Duration(0.1), onUpdate);
	ros::Timer timer = nh.createTimer(ros::Duration(0.1), &CSoko_Thinker::timerCallback,this);
	///TODO START LOGIC

	occ_grid_topic = "map";
	occ_grid_sub = nh.subscribe(occ_grid_topic.c_str(), 1, &CSoko_Thinker::mapCallback, this);

	bool solved = turn(map_table, robots_pos, boxes_pos, deliverys_pos, moves, 0);
	if(!solved)
	{
		ROS_DEBUG("MAP has no possible solution");
		exit(-1);
	}

	updateCurrentMovements();

	if(CSOKO_THINKER_DEBUG)
	{

		do
		{

			for(auto r_nr : robotsToMoveNow())
				moveRobotOnce(r_nr);


			frame.signalUpdate(grid,objects);
			//printBoard(t2);
			sleep(1);


		} while(current_moves.size() > 0);

		ROS_DEBUG("NO MORE MOVEMENTS");

	}
}


/**
 *
 */
CSoko_Thinker::~CSoko_Thinker(void)
{

}


/**
 *
 */
void CSoko_Thinker::onUpdate(const ros::TimerEvent&)
{
	ros::spinOnce();
	updateMap();
	update();
	if (!ros::ok())
	{
		void onUpdate();
	}
}



/**
 *
 */
void CSoko_Thinker::timerCallback(const ros::TimerEvent& e)
{
	ROS_DEBUG("CALLBACK");
	ROS_INFO("Timer callback!");
}


/**
 *
 */
void CSoko_Thinker::updateMap() {
	//TODO NAVIGATION LOGIC
}



/**
 *
 */
void CSoko_Thinker::update()
{
	//	map.drawAll(frame);
}


/**
 *
 */
void CSoko_Thinker::mapCallback(const nav_msgs::OccupancyGrid& msg){
	this->occ_grid_msg = msg;
	ROS_INFO("received map!");

	this->map_height = msg.info.height;
	this->map_width = msg.info.width;
	ROS_INFO("map_height: %i\nmap_width: %i", this->map_height, this->map_width);



	this->my_map = new int8_t*[this->map_height];
	for(int i = 0; i < this->map_height; i++){
		my_map[i] = new int8_t[this->map_width];

		for(int j = 0; j < this->map_width; j++){
			my_map[i][j] = msg.data.at(i*j);
		}
	}

	if(CSOKO_THINKER_DEBUG){
		ROS_INFO("CSOKO_THINKER_DEBUG");
		try{

			std::ofstream outfile ("map.csv");
			for(int i = 0; i < this->map_height; i++){
				for(int j = 0; j < this->map_width; j++){
					outfile << (int) my_map[i][j] << ",";
				}
				outfile << std::endl;
			}
			outfile.close();
		} catch(...){
			ROS_INFO("Exception!");
		}

	}
}



/**
 *
 */
void CSoko_Thinker::callback(const sensor_msgs::LaserScan& msg)
{
	laser_scan_msg = msg;
	float linear = 0, rotational = 0;
	for(unsigned int i = 0 ; i < laser_scan_msg.ranges.size() ; i++)
	{
		float real_dist = laser_scan_msg.ranges[i];
		linear -= cos(laser_scan_msg.angle_min + i * laser_scan_msg.angle_increment) / (1.0 + real_dist * real_dist);
		rotational -= sin(laser_scan_msg.angle_min + i * laser_scan_msg.angle_increment) / (1.0 + real_dist * real_dist);
	}
	geometry_msgs::Twist cmd;

	linear /= laser_scan_msg.ranges.size();
	rotational /= laser_scan_msg.ranges.size();

	//~ ROS_DEBUG("%f %f",linear,rotational);

	if(linear > 0.3)
	{
		linear = 0.3;
	}
	else if(linear < -0.3)
	{
		linear = -0.3;
	}

	cmd.linear.x = (odom_state < 3) ? 0.3 + linear : 0;
	cmd.angular.z = (odom_state > 1) ? 0.0174532925 * 10 : rotational;

	cmd_vel_pub.publish(cmd);
}



/**
 *
 */
void CSoko_Thinker::odometryCallback(const nav_msgs::Odometry msg){
	if(odom_state == 0){
		odom_msg = msg;
		odom_state = 1;
		return;
	}

	float currentXPos = msg.pose.pose.position.x;
	float currentYPos = msg.pose.pose.position.y;
	float oldXPos = odom_msg.pose.pose.position.x;
	float oldYPos = odom_msg.pose.pose.position.y;
	float delta = 0.0000001;

	bool stuckX = ((currentXPos - oldXPos) == 0) ? true : false;
	bool stuckY = ((currentYPos - oldYPos) == 0) ? true: false;

	float dx = std::abs(currentXPos - oldXPos),
			dy = std::abs(currentYPos - oldYPos);

	if(stuckX && stuckY){

		if(odom_state == 1) odom_state = 2;
		else if(odom_state == 2)  odom_state = 3;
		else if(odom_state == 3) odom_state = 2;

		ROS_INFO("dx = %f ; dy =  %f ; odomState= %i", dx, dy, odom_state);
	}
	else{
		odom_state = 1;
		ROS_INFO("dx = %f ; dy =  %f ; odomState= %i", dx, dy, odom_state);
	}
	//odom_msg.pose.pose.position.z = 0.0;

	//save it
	odom_msg = msg;
}



/**
 *
 */
void CSoko_Thinker::loadMap(string mapName)
{
	string line;
	int row = 0;
	string mapFilePath = mapName;
	frame.loadMap(mapName);

	ifstream mapFile(mapFilePath.c_str());
	if(mapFile.is_open())
	{
		while(getline(mapFile,line))
		{
			TableRow tr;
			std::vector<CSokoTile> mapRow;
			for(size_t i=0;i<line.length(); i++)
			{
				tr.push_back(line[i]);
				if(line[i] == ' ')
				{
					mapRow.push_back(CSokoTile(i,row,false, false));
				}
				else if(line[i] == '#')
				{
					mapRow.push_back(CSokoTile(i,row,false, true));
				}
				else if(line[i] == '@')
				{
					cout << "Found Robot at " << i << "/" << row << endl;
					CSokoTile tile = CSokoTile(i,row,false, false);
					CSokoObject r(i,row,false);
					objects.push_back(r);
					tile.setObject();
					mapRow.push_back(tile);
					T_pos rob_pos = make_tuple(i, row);
					robots_pos.push_back(rob_pos);
				}
				else if(line[i] == '$')
				{
					CSokoTile tile = CSokoTile(i,row,false, false);
					CSokoObject b(i,row,true);
					objects.push_back(b);
					tile.setObject();
					mapRow.push_back(tile);
					T_pos box_pos = make_tuple(i, row);
					boxes_pos.push_back(box_pos);
				}
				else if(line[i] == '.')
				{
					CSokoTile tile(i,row, true, false);
					tile.setObject();
					mapRow.push_back(tile);
					T_pos del_pos = make_tuple(i, row);
					deliverys_pos.push_back(del_pos);
				}
			}
			grid.push_back(mapRow);
			map_table.push_back(tr);
			row++;
		}
		mapFile.close();





	}
	else {
		cout << "Unable to find map file." << endl;
	}
}



/**
 *
 */
void CSoko_Thinker::drawAll()
{

}


/**
 *
 */
int CSoko_Thinker::getRobotPosByNo(int number)
{
	int i;
	int skipped = 0;
	for(i=0;i<objects.size();i++)
	{
		if(!objects[i].isBox && number == skipped)
		{
			break;
		}
		else if(!objects[i].isBox)
			skipped++;
	}
	return i;
}


/**
 *
 */
int CSoko_Thinker::getBoxPosByCoord(int x, int y)
{
	int i=-1;
	for(i=0;i<objects.size();i++)
	{
		if(objects[i].isBox && objects[i].x==x && objects[i].y==y)
		{
			break;
		}
	}
	return i;
}

/**
 *
 */
bool CSoko_Thinker::robotInUse(const V_Robot_Move &current_moves, const Robot_Move &evaluate)
{
	for(auto r_mv : current_moves)
	{
		if(get<0>(r_mv) == get<0>(evaluate))
			return true;
	}
	return false;
}

/**
 *
 */
void CSoko_Thinker::moveRobotOnce(size_t r_index)
{

	for( int i = 0; i < current_moves.size(); i++)
	{
		auto r_mv = current_moves.at(i);
		int r_nr = get<0>(r_mv);

		if(r_nr != r_index)
			continue;


		T_pos box_pos = make_tuple(numeric_limits<size_t>::max(), numeric_limits<size_t>::max());
		int rob_pos = matchRobotObj(r_nr);
		int cur_robX = objects[rob_pos].x, cur_robY = objects[rob_pos].y;
		int cur_robXD = objects[rob_pos].drawX, cur_robYD = objects[rob_pos].drawY;

		ROS_DEBUG("r:nr= %i and rob_pos = %i", r_nr, rob_pos);
		ROS_DEBUG("(x, y) = (%i, %i) ", cur_robX, cur_robY);
		ROS_DEBUG("(xD, yD) = (%i, %i)", cur_robXD, cur_robYD);

		int dx = 0, dy = 0;
		getMovementDelta(get<1>(r_mv).at(0), dx, dy);

		size_t future_x = objects[rob_pos].x + dx;
		size_t future_y = objects[rob_pos].y + dy;

		//is this robot the owner (or can be) of next tile?
		if(!grid[future_y][future_x].isLockedTo(r_nr))
			if(!lockPath(r_mv))
				break;


		performOneMove(map_table, robots_pos.at(r_nr), box_pos, get<1>(r_mv).at(0));

		if(get<0>(box_pos) != numeric_limits<size_t>::max())
		{
			int box_pos = getBoxPosByCoord(get<0>(robots_pos.at(r_nr)), get<1>(robots_pos.at(r_nr)));
			objects[box_pos].x += dx;
			objects[box_pos].drawX += dx;

			objects[box_pos].y += dy;
			objects[box_pos].drawY += dy;
		}

		objects[rob_pos].x += dx;
		objects[rob_pos].drawX += dx;

		objects[rob_pos].y += dy;
		objects[rob_pos].drawY += dy;

		get<1>(r_mv).erase(get<1>(r_mv).begin());
		get<1> (current_moves[i]).erase(get<1> (current_moves[i]).begin());

		if(get<1>(r_mv).size() == 0)
			current_moves.erase(current_moves.begin()+i);


		int debugValue = (int)current_moves.size();
		ROS_DEBUG("current_moves size = %i", debugValue );

	}


	updateCurrentMovements();
}

void CSoko_Thinker::updateCurrentMovements()
{
	for(int i = 0; i < moves.size(); i=0)
	{
		auto r_mv = moves.at(i);
		if(robotInUse(current_moves, r_mv))
			break;

		moves.erase(moves.begin());
		current_moves.push_back(r_mv);
	}
}

/**
 *
 */
size_t CSoko_Thinker::matchRobotObj(size_t r_index)
{
	for(auto rIndex_oIndex : this->objPos_rob_index)
	{
		if(get<1>(rIndex_oIndex) == r_index)
			return get<0>(rIndex_oIndex);
	}

	index_matcher im = make_tuple(getRobotPosByNo(r_index), r_index);
	objPos_rob_index.push_back(im);

	return get<0>(im);

}

/**
 *
 */
size_t CSoko_Thinker::matchObjRobot(size_t obj_index)
{

	for(auto rIndex_oIndex : this->objPos_rob_index)
	{
		if(get<0>(rIndex_oIndex) == obj_index)
			return get<1>(rIndex_oIndex);
	}

	return numeric_limits<size_t>::max();
}



/**
 *
 */
vector<size_t> CSoko_Thinker::robotsToMoveNow()
{
	vector<size_t> robots;
	for(auto r_mv : current_moves)
		robots.push_back(get<0>(r_mv));

	return robots;
}

bool CSoko_Thinker::lockPath(const Robot_Move &r_mv)
{
	size_t r_nr = get<0>(r_mv);
	string mv(get<1>(r_mv));
	XY_COORD pos = robots_pos[r_nr];

	return lockPath(r_nr, pos, mv);
}

bool CSoko_Thinker::lockPath(const size_t &r_nr, const XY_COORD &pos, string &mv)
{
	//no more tiles to lock?
	if(mv.size() == 0) return true;

	int dx =0, dy= 0;
	getMovementDelta(mv.at(0), dx, dy);
	size_t new_x = get<0>(pos) + dx, new_y =get<1>(pos) + dy;

	//is this robot is able to lock the next tile?
	if(!grid[new_y][new_x].increaseLock(r_nr)) return false;

	XY_COORD new_pos = make_tuple(new_x, new_y);
	mv.erase(mv.begin());

	//is this robot able to lock all tiles after this?
	if(!lockPath(r_nr, new_pos, mv))
	{
		grid[new_y][new_x].decreaseLock(r_nr);
		return false;
	}

	return true;

}

}
