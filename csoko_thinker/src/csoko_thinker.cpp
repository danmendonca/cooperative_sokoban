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

	string res_path(argv[1]);
	string name = res_path + argv[2];

	CSokoFrame::setPathToResources(res_path);

	ROS_INFO("MAP NAME: %s\n", name.c_str());

	CSokoFrame::setPathToResources(res_path);

	loadMap(name);

	ros::Timer timer = nh.createTimer(ros::Duration(0.1), &CSoko_Thinker::timerCallback,this);

	bool solved = turn(map_table, robots_pos, boxes_pos, deliverys_pos, moves, 0);

	if(!solved)
	{
		ROS_ERROR("MAP has no possible solution. Ctrl+C to exit.");
		exit(-1);
	}

	updateCurrentMovements();

	do
	{

		for(auto r_nr : robotsToMoveNow())
			moveRobotOnce(r_nr);

		frame.signalUpdate(grid,objects);

	} while(current_moves.size() > 0);

	ROS_DEBUG("NO MORE MOVEMENTS");


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
	ROS_DEBUG("Timer callback!");
}


/**
 *
 */
void CSoko_Thinker::updateMap()
{
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
					ROS_DEBUG("Found Robot at (x,y) = (%i, %i)", (int) i, (int) row);
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
			return i;
		}
	}
	return -1;
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

	updateCurrentMovements();
	ROS_DEBUG("current_movements size = %i", (int) current_moves.size());

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

		int dx = 0, dy = 0;
		getMovementDelta(get<1>(r_mv).at(0), dx, dy);

		size_t future_x = (get<0> (robots_pos[r_nr])) + dx;
		size_t future_y = get<1> (robots_pos[r_nr]) + dy;

		//is this robot the owner (or can be) of next tile?
		if(!grid[future_y][future_x].isLockedTo(r_nr))
		{
			if(!lockPath(r_mv))
			{
				ROS_DEBUG("COULD NOT LOCK PATH TO ME. r_nr: %i", r_nr);
				break;
			}
		}



		int box_index = getBoxPosByCoord(future_x, future_y);
		bool skip = false;

		if(objects[rob_pos].mState == STATE_MOV_NONE ||
				(box_index > 0 && objects[box_index].mState == STATE_MOV_NONE ))
		{
			skip = true;
			ROS_DEBUG("STATE_MOV_NONE");
			objects[rob_pos].addMove(dx, dy);
			if(box_index != -1)
			{
				objects[box_index].addMove(dx, dy);
			}
		}
		else if(objects[rob_pos].mState == STATE_MOV_PROGRESS ||
				(box_index > 0 && objects[box_index].mState == STATE_MOV_PROGRESS ))
		{
			bool isMoveBox = objects[rob_pos].updateDrawCoord();
			ROS_DEBUG("STATE_MOV_PROGRESS");
			if(box_index != -1 && isMoveBox)
			{
				objects[box_index].updateDrawCoord();
			}
		}
		else if(objects[rob_pos].mState == STATE_MOV_FINISHED)
		{
			ROS_DEBUG("STATE_MOV_FINISHED");
			objects[rob_pos].mState = STATE_MOV_NONE;
			grid[get<1>(robots_pos[r_nr])][get<0>(robots_pos[r_nr])].decreaseLock(r_nr);
			performOneMove(map_table, robots_pos.at(r_nr), box_pos, get<1>(r_mv).at(0));
			get<1>(r_mv).erase(get<1>(r_mv).begin());
			get<1> (current_moves[i]).erase(get<1> (current_moves[i]).begin());

			if(get<1>(r_mv).size() == 0)
				current_moves.erase(current_moves.begin()+i);


			int debugValue = (int)current_moves.size();
			ROS_DEBUG("current_moves size = %i", debugValue );
		}

	}


	updateCurrentMovements();
}

void CSoko_Thinker::updateCurrentMovements()
{
	for(int i = 0; i < moves.size();)
	{
		auto r_mv = moves.at(i);
		if(robotInUse(current_moves, r_mv)){
			i++;
			continue;
		}

		current_moves.push_back(moves[i]);
		moves.erase(moves.begin() + i);
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
