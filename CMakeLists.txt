cmake_minimum_required(VERSION 3.0.2)
project(gazebo_wireless)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  gazebo_ros
  message_generation
)

find_package(gazebo REQUIRED)


catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS gazebo_ros message_runtime roscpp
)

include_directories(
  include ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS}
)

link_directories(${GAZEBO_LIBRARY_DIRS})



add_executable(wireless_listen src/wireless_listen.cpp)
target_link_libraries(wireless_listen ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})


