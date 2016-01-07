Installation Instructions

Necessary Software:
-Ubuntu 14.04
-ROS Indigo version > sudo apt-get install ros-indigo-desktop-full
-sfml library > sudo apt-get install libsfml-dev


Instructions:
-Create a catkin workspace (we will refer to it as catkin_ws)
-Create a folder inside catkin_ws named src
-Move the folder cooperative_sokoban that was extracted with this file, into catkin/src
-In terminal, change directory to catkin_ws and do as follows:
>catkin_make
>source devel/setup.bash
-Now you can run the one of the two launchers that exist with this package, choosing one of the following:
>roslaunch csoko_launchers csoko_thinker_complex.launch
>roslaunch csoko_launchers csoko_thinker_base.launch