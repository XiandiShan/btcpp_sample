cmake_minimum_required(VERSION 3.5)

project(btcpp_sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(btcpp_sample main.cpp)
add_executable(btcpp_loop_sample loop_sample.cpp)
add_executable(select_points select_points.cpp)


find_package(ament_cmake QUIET)

if(ament_cmake_FOUND)

    find_package(behaviortree_cpp REQUIRED)
    ament_target_dependencies(btcpp_sample behaviortree_cpp)
    ament_target_dependencies(select_points behaviortree_cpp)
    ament_target_dependencies(btcpp_loop_sample behaviortree_cpp)


elseif( CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)

    find_package(catkin REQUIRED COMPONENTS behaviortree_cpp)
    catkin_package(CATKIN_DEPENDS behaviortree_cpp)
    target_include_directories(btcpp_sample btcpp_loop_sample select_points PRIVATE ${catkin_INCLUDE_DIRS})
    target_link_libraries(btcpp_sample btcpp_loop_sample select_points ${catkin_LIBRARIES})

else()

    find_package(behaviortree_cpp REQUIRED)
    target_link_libraries(btcpp_sample btcpp_loop_sample select_points BT::behaviortree_cpp)

endif()

