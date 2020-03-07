cmake_minimum_required(VERSION 2.8.3)
project(detected_person_association)

find_package(catkin REQUIRED COMPONENTS rospy roscpp message_filters tracking_msgs std_msgs message_generation nodelet eigen_conversions tf_conversions cmake_modules)


FIND_PACKAGE(Eigen REQUIRED)
include_directories(${Eigen_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})

add_service_files(
  FILES
  LookupTrackId.srv
  LookupTrackIds.srv
)

catkin_python_setup()

generate_messages(
   DEPENDENCIES
   std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp message_filters tracking_msgs std_msgs nodelet eigen_conversions tf_conversions
)

link_directories(${catkin_LIBRARY_DIRS})
set(SOURCE_DIR src/detected_person_association)


### Nodelet library for fusion of detected persons ###
add_library(${PROJECT_NAME}
  ${SOURCE_DIR}/convert_to_composite_detections.cpp
  ${SOURCE_DIR}/composite_detected_persons_synchronizer.cpp
  ${SOURCE_DIR}/aggregate_detections.cpp
  ${SOURCE_DIR}/nn_fuser.cpp
  ${SOURCE_DIR}/euclidean_nn_fuser.cpp
  ${SOURCE_DIR}/polar_nn_fuser.cpp
)
add_dependencies(${PROJECT_NAME} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
  PATTERN ".svn" EXCLUDE
)

install(FILES nodelet_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

catkin_install_python(PROGRAMS
  scripts/check_poses_and_covariances.py
  scripts/composite_detections_to_detections.py
  scripts/dump_detection_positions.py
  scripts/track_synchronizer_test.py
  scripts/visualize_composite_detections.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
