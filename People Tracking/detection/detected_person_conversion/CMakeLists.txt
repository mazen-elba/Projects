cmake_minimum_required(VERSION 2.8.3)
project(detected_person_conversion)

find_package(catkin REQUIRED COMPONENTS rospy tracking_msgs)

catkin_package(
  CATKIN_DEPENDS rospy tracking_msgs
)
