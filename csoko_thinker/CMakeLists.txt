cmake_minimum_required(VERSION 2.8.3)
project(csoko_thinker)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	tf
	stdr_msgs
	geometry_msgs
	sensor_msgs
	nav_msgs
 )


find_package(Qt4 4.6 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})

################################################
## Declare ROS messages, services and actions ##
################################################


################################################
## Declare ROS dynamic reconfigure parameters ##
################################################



###################################
## catkin specific configuration ##
###################################
catkin_package(
	INCLUDE_DIRS 
    	include
  	LIBRARIES 
  	CATKIN_DEPENDS
    	roscpp
    	tf
    	stdr_msgs
    	geometry_msgs
    	sensor_msgs
    	nav_msgs
)

qt4_wrap_cpp(turtlesim_node_MOCS ${turtlesim_node_HDRS})

###########
## Build ##
###########

#set(CMAKE_BUILD_TYPE Release) #TODO remove this line or uncomment?

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)

add_executable(csoko_thinker src/csoko_thinker.cpp src/csoko_frame.cpp src/csoko_object.cpp src/csoko_tile.cpp src/csoko_box.cpp src/csoko_robot.cpp)
add_dependencies(csoko_thinker stdr_msgs_gencpp) # wait for stdr_msgs to be build

target_link_libraries(csoko_thinker 
	${catkin_LIBRARIES} ${QT_LIBRARIES}
)

#############
## Install ##
#############

install(TARGETS csoko_thinker
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY csoko_images
DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
FILES_MATCHING PATTERN "*.png" PATTERN "*.svg")

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_csoko_thinker.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
