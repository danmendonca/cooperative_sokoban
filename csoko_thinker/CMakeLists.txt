cmake_minimum_required(VERSION 2.8.3)
project(csoko_thinker)

add_definitions(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	tf
 )


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
		sfml 
  	CATKIN_DEPENDS
    	roscpp
    	tf
)


###########
## Build ##
###########

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)

add_executable(csoko_thinker src/csoko_thinker.cpp src/csoko_frame.cpp
 	src/csoko_object.cpp src/csoko_tile.cpp src/csoko_solver.cpp)

target_link_libraries(csoko_thinker 
	${catkin_LIBRARIES} sfml-graphics sfml-window sfml-system
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
